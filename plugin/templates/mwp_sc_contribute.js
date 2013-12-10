jQuery(document).ready(function() {
	jQuery('.wp-submit').click(function() {
		jQuery.post( jQuery('.action_url').val(), function( data ) {
			jQuery(".payForm" ).html( data );
			jQuery('.payForm').toggle();
		});
	});

	jQuery('.user_type').click(function() {
		jQuery('.mangopay_natural').toggle();
		jQuery('.mangopay_legal').toggle();
	});
});

