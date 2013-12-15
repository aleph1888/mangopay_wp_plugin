jQuery(document).ready(function() {
	function change_user_display () {
		if ( jQuery('.user_type').is(":checked") ) {
			jQuery('.mangopay_legal').show();
			jQuery('.mangopay_natural').hide();
		} else {
			jQuery('.mangopay_natural').show();
			jQuery('.mangopay_legal').hide();
		}
	}
	jQuery('.user_type').click(function() {	
		change_user_display ();
	});
	jQuery('.bt_change_user_data').click(function() {
		jQuery('.mangopay_userheader').show();
		change_user_display ();
		jQuery('.bt_change_user_data').hide();
	});
	jQuery('.bt_register_card').click(function() {
		jQuery('.mangopay_cards').show();
		jQuery('.bt_register_card').hide();
		jQuery('.Alias').html('--');
	});
});

