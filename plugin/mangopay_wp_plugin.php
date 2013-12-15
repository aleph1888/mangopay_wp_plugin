<?php
/*
Plugin Name: Mangopay wp plugin
Plugin URI: http://www.github.com/aleph1888/mangopay_wp_plugin
Description: Simple Mangoypay Users-buyers & users-sellers implementation.
Version: 0.1
Author: Hackafou
Author URI: http://www.coopfunding.net
Text Domain: mangopay_wp_plugin
Domain Path: /languages/
*/

add_action( 'init', 'mwp_init' );

function mwp_init() {

	require_once __DIR__ . "/includes/mwp_api.inc";
	require_once __DIR__ . "/includes/mwp_user.inc";
	require_once __DIR__ . "/includes/mwp_user.inc";
	require_once __DIR__ . "/includes/mwp_bank.inc";
	require_once __DIR__ . "/includes/mwp_fields.inc";
	require_once __DIR__ . "/includes/mwp_forms.inc";

	 wp_register_style( 'mwp_sc_contribute_css', plugins_url('templates/mwp_sc_contribute.css', __FILE__) );
	 wp_register_script ( "mwp_sc_contribute_js", plugins_url( 'templates/mwp_sc_contribute.js', __FILE__ ) , array( 'jquery' ) );

	//Language
	load_plugin_textdomain( 'mangopay_wp_plugin', false, dirname(plugin_basename(__FILE__)) . '/languages/' );

	//Configuration of Mangopay in profile. This is, minimum for users that are autors of posts. This user will owner the wallet.
	include (__DIR__ . "/mwp_profile.php");

	//Contribute Shortcode
	include (__DIR__ . "/mwp_sc_contribute.php");

	//Raised Shorcode
	include (__DIR__ . "/mwp_sc_raised.php");

	//Withdraw metabox in post edition sidebar
	include (__DIR__ . "/mwp_post.php");

}

add_action('init', 'myStartSession', 1);
add_action('wp_logout', 'myEndSession');
add_action('wp_login', 'myEndSession');

function myStartSession() {
	if(!session_id()) {
		session_start();
	}
}

function myEndSession() {
	session_destroy ();
}

