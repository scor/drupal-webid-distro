(function ($) {

Drupal.behaviors.webidauth = {
  attach: function (context) {
    var loginElements = $('.form-item-name, .form-item-pass, li.webidauth-link');
    var cookie = $.cookie('Drupal.visitor.webidauth_identifier');
    
    // This behavior attaches by ID, so is only valid once on a page.
    if (!$('#edit-webidauth-identifier.webidauth-processed').size()) {
      if (cookie) {
        $('#edit-webidauth-identifier').val(cookie);
      }
      if ($('#edit-webidauth-identifier').val() || location.hash == '#webidauth-login') {
        $('#edit-webidauth-identifier').addClass('webidauth-processed');

        // Use .css('display', 'block') instead of .show() to be Konqueror friendly.
        webidauthElements.css('display', 'block');
      }
    }


  }
};

})(jQuery);
