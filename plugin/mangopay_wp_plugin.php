<?php
/*
Plugin Name: Mangopay wp plugin
Plugin URI: http://www.github.com/aleph1888/mangopay_wp_plugin
Description: Simple Mangoypay Users-buyers & users-sellers implementation.
Version: 0.1
Author: Hackafou
Author URI: http://www.coopfunding.net
Text Domain: mangopay_wp_plugin
Domain Path: /language/
*/

//Language
include (dirname(__FILE__) . "/mangopay_wp_plugin_languages.php");

//Profile fields
include (dirname(__FILE__) . "/mangopay_wp_plugin_profile_fields.php");

//Withdraw metabox in post edition sidebar
include (dirname(__FILE__) . "/mangopay_wp_plugin_post_fields.php");

//Contribute Shortcode
include (dirname(__FILE__) . "/mangopay_wp_plugin_contribute_shortcode.php");

//Raised Shorcode
include (dirname(__FILE__) . "/mangopay_wp_plugin_raised_shortcode.php");




