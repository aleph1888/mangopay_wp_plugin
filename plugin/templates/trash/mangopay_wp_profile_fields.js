jQuery(document).ready(function() {
	jQuery('.user_type').click(function() {
		jQuery('.mangopay_natural').toggle();
		jQuery('.mangopay_legal').toggle();
	});

	jQuery('.bt_reg_card').click(function() {
		alert();
		jQuery.post( jQuery('.bt_reg_card').attr('url'), function( data ) {
			//$( ".result" ).html( data );
			alert(data);
		});
	});


});

