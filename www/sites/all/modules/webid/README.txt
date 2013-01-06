INTORDUCTION
------------
WebID Authorization is a module that enables a user to log in a Drupal 7.x site using a client
certificate (per the WebID specification).

This module depends on the ARC2 library and libAthentication by Melvin Carvalho. To fully 
control the authentication process, you will also need the WebIDauth service.

FEAUTURES
---------
After installing and enabling the module, your users will be able to:
1. Register in your Drupal site via WebID ('Login with WebID' link in the registration block)
2. Associate your existing account with a WebID ('WebID identities' tab in user profile view)

You, as an administrator will, also, be able to:
1. Register a new user with a WebID (Add user page in People panel, password is optional if you 
enter a WebID)
2. Associate any account with a WebID (Edit user page - 'WebID identities' tab)

Self - associated WebID identities are verified immediately, while associated identities for other 
users are verified as soon as they try to login using them.

DEPENDENCIES
------------
1. Libraries API Module (http://drupal.org/project/libraries)
2. libAuthentication (WebIDauth fork: https://github.com/WebIDauth/libAuthentication)
3. ARC2 library (https://github.com/semsol/arc2/)
4. Graphite RDF library (http://graphite.ecs.soton.ac.uk/)
5. A working installation of WebIDauth (https://github.com/WebIDauth/WebIDauth)

Dependencies #2, #3 and #4 should be installed in the  /sites/all/libraries directory. The path to
libAuthentication's config.php should be
/sites/all/libraries/libAuthentication/config.php , while the path to ARC's ARC2.php should be 
sites/all/libraries/ARC2/arc/ARC2.php , per the rdfx module recommendation. The path to
graphite's  Graphite.php should be sites/all/libraries/Graphite/Graphite.php

Dependency #5 requires a correctly configured https virtual host on your server. You can also 
use an  already installed service, like the one in https://auth.fcns.eu/auth/index.php
The WebIDauth service URL should be entered in the module's configuration, in 
admin/config/people/webidauth.

Note: After installing WebIDauth and libAuthentication, you have to provide the public key of 
your WebIDauth service to libAuthentication. For more information please read INSTALL.txt

ABOUT
-----
Maintainers: Lazaros Ioannidis (larjohn), Charalampos Bratsas (cbratsas), Dimitris Spachos 
(dimitris.spachos)

Supported by the mEducator Project
http://www.meducator.net/

