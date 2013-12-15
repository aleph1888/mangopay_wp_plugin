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
				"id_name" => 'form' .  substr(str_shuffle("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"), 0, 5),
				"login" => mwp\mwp_forms::mwp_show_wordpress_login()
	);

	$output .= file_get_contents( $template_url );
	foreach ($yTemplate as $key => $value) 
		$output = str_replace ( "%%{$key}", $value, $output );
	
	return $output;

}
