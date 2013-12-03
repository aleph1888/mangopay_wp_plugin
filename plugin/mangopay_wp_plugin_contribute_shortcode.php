<?php

/**
* Add CONTRIBUTE shortcode to any post by:
*	[mwp_contribute amount="999" post_id="0"] 
*	If is missing post_id or is set to 0, then will assume that shortcode is located on post
*	If is missing amount or is set to 0, then will show inputbox
**/

add_shortcode( 'mwp_contribute', 'mwp_show_contribute' );

function mwp_show_contribute( $atts ) {

	//Get params
	extract( shortcode_atts( array(
		'post_id' => '0',
		'amount' => '0'
	), $atts ) );

	//User gatekeeper
	$user = wp_get_current_user();
	if ( $user -> ID == 0) {
		$url = get_bloginfo('url') . "/wp-login.php";
		$output = "<a href={$url}>" . __('login_to_contribute', "mangopay_wp_plugin") . "</a>";
	} else {
		//If there is not post_id attribute, get current post.
		if ($post_id == 0) {
			global $post;
			$post_id = $post -> ID;
		}

		//Get post wallet_id or create a new one.
		$wallet_id = get_post_meta($post_id, "wallet_id", 1);
		if (!$wallet_id) {
			require_once (dirname(__FILE__) . "/mangopay/lib/common.inc");
			$body = json_encode(array("Owners" => array($post->post_author), "Tag" => $post_id));
			$wallet = request("wallets", "POST", $body);
			if (isset($wallet) && isset($wallet -> ID)) {
				$wallet_id = $wallet -> ID;
				add_post_meta($post_id, "wallet_id", $wallet_id, 1);
			} else {
				return "ERROR Could not create a wallet for this project.";
			}
		}
		
		//Choose template according to amount
		if (isset($amount) && $amount > 0 ) {
			$template = "mpw_contribute_shortcode.html";
			$amount_caption = $amount . __( "eur", 'mangopay_wp_plugin');
		} else {
			$template = "mpw_contribute_shortcode_amount.html";
			$amount_caption = __( "amount", 'mangopay_wp_plugin');
		}

		//Call template
		$path =  plugin_dir_url( __FILE__ ) . 'mangopay/';
		$output = file_get_contents($path . $template);
		$output = str_replace ("%%title_caption",  __( "contributions", 'mangopay_wp_plugin'), $output);
		$output = str_replace ("%%user_id",  $user->ID, $output);
		$output = str_replace ("%%wallet_id",  $wallet_id, $output);
		$output = str_replace ("%%action_url",  $path . 'mwp_contribute.php', $output);
		$output = str_replace ("%%form_name",  "mwp_frm_contribute{$wallet_id}", $output);
		$output = str_replace ("%%return_url", get_permalink($post_id), $output);
		$output = str_replace ("%%amount_caption", $amount_caption, $output);
		$output = str_replace ("%%amount",  $amount, $output);
		$output = str_replace ("%%contribute_caption",  __( "contribute", 'mangopay_wp_plugin'), $output);
		$output = str_replace ("%%template_url",  $path . "mwp_payment_skeleton.html", $output);
	}
	
	return $output;

}
