<?php

//edit wp_config.php, param: WPLANG, to establish proper .mo file sufix.
function mangopay_wp_plugin_init() {
	$path = dirname(plugin_basename(__FILE__)) . '/languages/';
	load_plugin_textdomain( 'mangopay_wp_plugin', false, $path );

}

add_action( 'plugins_loaded', 'mangopay_wp_plugin_init' );
