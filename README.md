Drupal WebID distribution
=========================

This distribution exists to get people interested in WebID and Drupal started
quickly. It includes all the necessary modules and third party libraries
required to:
* use Drupal as a WebID provider allowing you to authenticate to other WebID
enabled sites
* register and/or log in Drupal using your existing WebID (which may or may
not be provided by Drupal at the first place)

In particular, this distribution includes the
[Drupal WebID module](http://drupal.org/project/webid)
and the third party libraries [libAuthentication](https://github.com/melvincarvalho/libAuthentication),
[ARC2](https://github.com/semsol/arc2) and [Graphite](http://graphite.ecs.soton.ac.uk/).
You can of course download each of these separately to your existing Drupal site.
The versions of the libraries packaged here are known to work with Drupal,
so this distribution provides a good testbed for evaluating WebID in Drupal.

## Advanced WebID use case example

While the main use case WebID has seen so far is in public social networks such as MyProfile, we demonstrate how it is possible to leverage the WebID protocol in enterprise scenarios where several applications hosted on different platforms are at play. We showcase an imaginary set of health related applications (*.happs.gov) whose members are able to seamlessly register and sign on using a dedicated WebID provider part of the happs.gov network.

This infrastructure is available for demo purposes and can be set up using VirtualBox. The Virtual Machines have been uploaded online and can be downloaded from the following locations:
* [drupal_webid_distro_vm.ova.tar.gz](http://files.openspring.net/2013/h2challenge/drupal_webid_distro_vm.ova.tar.gz)
* [OntoWiki_vm.ova.tar.gz](http://files.openspring.net/2013/h2challenge/OntoWiki_vm.ova.tar.gz)
* [webIDauth_vm.ova.tar.gz](http://files.openspring.net/2013/h2challenge/webIDauth_vm.ova.tar.gz)

These virtual machines will most likely need to be tweaked to match the IPs assigned to them on your network in order to reproduce was is shown in the screencast. Unix accounts use the username "demo" and the password "demo" unless otherwise noted. MySQL root password is "root". Drupal admin password is "admin". These VMs should not be deployed on production server as they are not secure, they are for educational purpose only.

### Set up
Download the VMs above and deploy them in [VirtualBox](https://www.virtualbox.org/). The Virtual Machines need to be able to have access to each other via the ports 80 and 443. Using a Bridged adapter as Network "attached to" will allow each VM to get an IP from your network if it supports DHCP. You will need to update the /etc/hosts files of your VM to something like this:
````
192.168.1.123 webid.happs.gov
192.168.1.134 auth.happs.gov
192.168.1.123 drupal.happs.gov
192.168.1.135 ontowiki.happs.gov
````
In the example above I used the same VM for both the webid and the drupal subdomains, but I had a vhost for each. Feel free to have them separate machines.
