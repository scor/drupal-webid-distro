<?php

/**
 * @copyright
 *     WebIDauth - WebID authentication module for Drupal
 *     Copyright (C) 2012  mEducator Best Practice Network
 * @author
 *      Lazaros Ioannidis
 *      Charalampos Bratsas
 *      Dimitris Spachos
 */

/**
 * @file
 * WebID authentication module supporting pages
 */

/**
 * Menu callback; Manage WebID identities for the specified user.
 */
function webidauth_user_identities($account)
{
    global $user;
    drupal_set_title(format_username($account));
    drupal_add_css(drupal_get_path('module', 'webidauth') . '/webidauth.css');


    $header = array(t('WebID'), t('Operations'));
    $rows = array();
    $user_data = db_select('webidauth', 'w');

    $user_data->fields('w', array('uri', 'wid'))->condition('w.uid', $account->uid,
        '=');

    $result = $user_data->execute();

    foreach ($result as $identity) {
        $rows[] = array(check_plain($identity->uri), l(t('Delete'), 'user/' . $account->
            uid . '/webidauth/delete/' . $identity->wid));
    }

    $build['webidauth_table'] = array('#theme' => 'table', '#header' => $header,
        '#rows' => $rows, );

    if ($account->uid !== $user->uid)
        $build['admin_webidauth_user_add'] = drupal_get_form('admin_webidauth_user_add');
    else
        $build['webidauth_user_add'] = drupal_get_form('webidauth_user_add');
    return $build;
}


/**
 * Menu callback; Add a WebID identity for the logged in user.
 *
 * @return
 */
function webidauth_add_page()
{

    global $user;
  $pubkey = array(variable_get('webidauth_auth_server_cert', DEFAULT_WEBID_VALIDATOR_CERTIFICATE));
  $auth = new Authentication_FoafSSLDelegate(false,NULL,NULL,new Authentication_X509CertRepo($pubkey));
    if (isset($_REQUEST['error'])) {
        if ($_REQUEST['error'] === 'noVerifiedWebId')
            drupal_set_message(t('The provided WebID could not be verified.'), 'error');
    }
    if ($auth->isAuthenticated()) {
        $uri = $_REQUEST['webid'];
        if ($uri) {
            $u = _webidauth_exists($uri);
            if ($u !== false) {

                drupal_set_message(t('The claimed WebID is already used by another user. Please
                contact ' . variable_get('site_name') . ' administrator.'),
                    'warning');
                drupal_goto('user/' . $user->uid . '/webidauth/');

            } else {


                _webidauth_save( $uri, $user->uid);

                drupal_set_message(t('The WebID was successfully associated with your account.'));
                drupal_goto('user/' . $user->uid . '/webidauth/');
            }

        }
    }

}


/**
 * Menu callback; Authenticate with a  WebID identity for the logged in user.
 *
 * @return
 */
function webidauth_authentication_page()
{
    global $user;

    // Package the certificate defined on the site so that it can be sent to
    // Authentication_X509CertRepo.
    $referer_host = Authentication_URL::parse(variable_get('webidauth_auth_server_uri', DEFAULT_WEBID_VALIDATOR))->host;
    $cert_repo = array($referer_host => variable_get('webidauth_auth_server_cert', DEFAULT_WEBID_VALIDATOR_CERTIFICATE));

    // Instantiate Authentication_FoafSSLDelegate() with our custom certificate.
    $auth = new Authentication_FoafSSLDelegate(false,NULL,NULL,new Authentication_X509CertRepo($cert_repo));

    if (isset($_REQUEST['error'])) {
        if ($_REQUEST['error'] === 'noVerifiedWebId' ||$_REQUEST['error']==='certNoOwnership' )
            drupal_set_message(t('The provided WebID cannot be verified successfully.'),
                'error');
                drupal_goto('<front>');
        return;
    }

    drupal_add_js(drupal_get_path('module', 'webidauth') . '/logout.js');

    if (user_is_logged_in())
        return;

    if ($auth->isAuthenticated()) {//successful WebID validation
        $uri = $_REQUEST['webid'];
        if ($uri) {
            $foaf = new FoafParser($uri);

            $u = _webidauth_exists($uri);
            if ($u !== false) {
                user_external_login_register(trim($u->name), 'webidauth');
                drupal_goto('<front>');

            } else {//create the account with the username & email if they don't already exist, else go to register page
                $existing_user_data = db_select('users', 'u');
                $existing_user_data->fields('u', array('name'))->condition(db_or()->condition('u.name', trim($foaf->getName()),
                    '=')->condition('u.mail', $foaf->getMailbox(), '='));

                $existing_user_obj = $existing_user_data->execute()->fetchObject();
                if ($existing_user_obj !== false ||
                     (trim($foaf->getName())== '' || trim($foaf->getMailbox())== '')
                ) {
                    $_SESSION['foaf'] = $uri;
                    drupal_goto('user/register');
                }
                else{
                    user_external_login_register(trim($foaf->getName()), 'webidauth');
                    if(user_is_logged_in()){
                        _webidauth_save($uri,$user->uid);
                        $user->mail = trim($foaf->getMailbox());
                        user_save($user);
                    }
                    drupal_goto('<front>');
                }
            }
        }
    }
    else {
      // Authentification failed, provide error message for diagnostic.
      drupal_set_message('WebID authentification failed: ' . $auth->authnDiagnostic, 'error');
      return '';
    }
}


/**
 * Menu callback; Add a WebID identity for the specified user.
 */
function admin_webidauth_user_add()
{
    $form['webidauth_identifier'] = array('#type' => 'textfield', '#title' => t('WebID FOAF-Profile URL'), );
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Add a WebID'));
    return $form;
}

/**
 * submit hook for the admin_webidauth_user_add form
 *
 * @param mixed $form
 * @param mixed $form_state
 * @return
 */
function admin_webidauth_user_add_submit($form, &$form_state)
{
    $return_to = url('user/' . arg(1) . '/webidauth/', array('absolute' => true));
    $uri = $form_state['values']['webidauth_identifier'];
    if ($uri) {


        $u = _webidauth_exists($uri);
        if ($u !== false) {
            drupal_set_message(t('The claimed WebID is already used by another user. Please
             review user <a href="user/' . $u->uid . '/webidauth/">' . $u->name .
                '</a> before continuing.'), 'warning');

        } else {


            _webidauth_save($uri, arg(1));
            drupal_set_message(t('The WebID was successfully associated with the selected account.'));
            drupal_goto('user/' . arg(1) . '/webidauth/');
        }

    }

}

/**
 * Form builder; Add an WebID identity.
 *
 * @ingroup forms
 * @see webid_user_add_validate()
 */
function webidauth_user_add()
{

    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Add a WebID'));
    return $form;
}


/**
 * submit hook for the webidauth_user_add form
 */
function webidauth_user_add_submit($form, &$form_state)
{
    $return_to = url('user/' . arg(1) . '/webidauth/add', array('absolute' => true));
    //$return_to = url('user/' . arg(1) . '/webidauth/', array('absolute' => true));

    global $base_url;
    drupal_goto(variable_get('webidauth_auth_server_uri', DEFAULT_WEBID_VALIDATOR) . '?authreqissuer=' . $return_to);
}

/**
 * Menu callback; Delete the specified WebID identity from the system.
 */
function webidauth_user_delete_form($form, $form_state, $account, $wid = 0)
{

    $user_data = db_select('webidauth', 'w');

    $user_data->fields('w', array('uri', 'wid'))->condition('w.wid', $wid, '=')->
        condition('w.uid', $account->uid, '=');
    $result = $user_data->execute()->fetchObject();

    $webid_count = db_select('webidauth', 'w');
    $webid_count->fields('w', array('uid'))->condition('w.uid', $account->uid, '=');
    $count_result = $webid_count->countQuery()->execute();
    $extra = '';
    if ($count_result->fetchField() < 2) {
        $extra = '%user has only one WebID left. If you delete it, %user will need to login with a valid password. ';
    }
    return confirm_form(array(), t($extra .
        'Are you sure you want to delete the WebID %authname for %user? ', array('%authname' =>
        $result->uri, '%user' => $account->name)), 'user/' . $account->uid .
        '/webidauth');
}

/**
 * submit hook for the webidauth_user_delete form
 */
function webidauth_user_delete_form_submit($form, &$form_state)
{

    $query = db_delete('webidauth')->condition('uid', $form_state['build_info']['args'][0]->
        uid)->condition('wid', $form_state['build_info']['args'][1])->execute();
    if ($query) {
        drupal_set_message(t('WebID deleted.'));
    }
    $form_state['redirect'] = 'user/' . $form_state['build_info']['args'][0]->uid .
        '/webidauth';
}
