<?php

/**
* Add RAISED shortcode to any post by:
*	[mwp_raised post_id="1"] 
*	If is missing post_id or is set to 0, then will assume that shortcode is located on post
**/

add_shortcode( 'mwp_raised', 'mwp_show_raised' );

function mwp_show_raised( $atts ) {
	//Get params
	extract( shortcode_atts( array(
		'post_id' => '0'
	), $atts ) );

	//If there is not post_id attribute, get current post.
	if ($post_id == 0) {
		global $post;
		$post_id = $post -> ID;
	}
	
	//Search for wallet
	$wallet_id = get_post_meta($post-> ID, "wallet_id", 1);
	if ($wallet_id) {
		require_once (dirname(__FILE__) . "/mangopay/lib/common.inc");
		$wallet = request("wallets/$wallet_id", "GET");
	}

	//Display info
	if (!$wallet || $wallet -> Amount == 0) {
		$output = __( "no_contributions", 'mangopay_wp_plugin');
	} else {
		$output = "<div><label for='amount'>" . __( "total", 'mangopay_wp_plugin' ) . ": " . $wallet -> Amount . __( "eur", 'mangopay_wp_plugin' ) . "</label></div>";
	}
	return $output;

}
