jQuery(document).ready(function () {
 /* two functions to enable TLS Login and Logout in javascript. Works
 for Internet Explorer and Firefox. Sadly missing in other browsers */

    if (document.all == null) {// FF, Opera, etc
       if (window.crypto) {
           try{
               window.crypto.logout();
               return false; //firefox ok -- no need to follow the link
           } catch (err) {//Safari, Opera, Chrome -- try with session breaking
           }
       } else { //Opera, will require server side session breaking
       }
    } else { // MSIE 6+
       document.execCommand('ClearAuthenticationCache');
       return false;
    };
    return true;
    });

