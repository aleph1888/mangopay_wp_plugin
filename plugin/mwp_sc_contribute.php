<?php
/**
* Add CONTRIBUTE shortcode to any post by:
*	[mwp_contribute amount="999" post_id="0"] 
*	If is missing post_id or is set to 0, then will assume that shortcode is located on post
*	If is missing amount or is set to 0, then will show inputbox
**/

//SHOW
add_shortcode( 'mwp_contribute', 'mwp_show_contribute' );

function mwp_show_contribute( $atts ) {

	//Get params
	extract( shortcode_atts( array(
		'post_id' => '0',
		'amount' => '0'
	), $atts ) );
	//If there is not post_id attribute, get current post.
	if ($post_id == 0) {
		global $post;
		$post_id = $post -> ID;
	}

	//Post author must have a valid mangopay user
	if ( ! get_userdata ( $post->post_author ) -> mangopay_id ) {
		_e ( 'mangopay_author_id_missing', 'mangopay_wp_plugin' );
		return;
	}

	$user = wp_get_current_user();
	
	//Contribute data url parms
	$url_params = "&post_id={$post_id}&amount={$amount}";

	//(A) Is logged
	if ( $user -> ID == 0 ) {
		$contribute_url =  site_url() . '/wp-admin/profile.php?mwp_action=user_data{$url_params}#mangopay_user' ;
		$action_url = site_url() . "/wp-login.php?action=register&redirecto_to={$contribute_url}";
	}

	//(B) Has payed before
	if ( !$action_url && !$user -> mangopay_id ) 
		$action_url = site_url() . "/wp-admin/profile.php?mwp_action=user_data{$url_params}#mangopay_user";

	//(C) Has ward
	if ( !$action_url && !$user -> registered_cards ) 
		$action_url = site_url() . "/wp-admin/profile.php?mwp_action=register{$url_params}#mangopay_cards";

	//Choose template according to amount
	if (isset($amount) && $amount > 0 ) {
		$template = "mwp_sc_contribute.html";
		$amount_caption = $amount . __( "eur", 'mangopay_wp_plugin');
	} else {
		$template = "mwp_sc_contribute_amount.html";
		$amount_caption = __( "amount", 'mangopay_wp_plugin');
	}
	$template_url =  plugin_dir_url( __FILE__ ) . "/templates/" . $template;

	//Config template
	$yTemplate = array ( "title_caption" =>  __( "contributions", 'mangopay_wp_plugin'),
				"amount_caption" => $amount_caption,
				"amount" => $amount,
				"action_url" => plugin_dir_url(__FILE__) . "mwp_payform.php",
				"post_id" => $post_id,
				"contribute_caption" => __( "contribute", 'mangopay_wp_plugin'),
	);

	$output .= file_get_contents( $template_url );
	foreach ($yTemplate as $key => $value) 
		$output = str_replace ( "%%{$key}", $value, $output );

	return $output;

}
