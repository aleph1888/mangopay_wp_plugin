<?php

function mwp_print_checkbox ( $name, $is_checked ) {

	$checked = ( $is_checked  ? " checked='checked' " : null );
	return mwp_print_row (
			mwp_print_tag ( 'label', $name ),
			"<input name='{$name}' type='checkbox' {$checked} class='{$name}' />".
			"<span class='description'>" . __( "{$name}_description", 'mangopay_wp_plugin') . "</span>"
	);

}

function mwp_print_label ( $key, $value, $show_description = null ) {
	return mwp_print_row (
		"<b>" . mwp_print_tag ('label', $key, $show_description) . "</b>",
		mwp_print_tag ('label', $value, $key) .
		( $show_description ? 
			"&nbsp;<span class='description'><i>" . __( "{$key}_description", 'mangopay_wp_plugin') . "</i></span>\n" : 
			null
		) );

}

function mwp_print_inputbox ( $key, $value ) {
	return mwp_print_row (
		mwp_print_tag ('label', $key ) ,
		"<input type='text' id='mwp_{$key}' name='mwp_{$key}' value='{$value}' class='regular-text'>&nbsp;" .
		"<span class='description'>" . __( "{$key}_description", 'mangopay_wp_plugin') . "</span>\n"
	);

}

function mwp_print_row ( $label, $field ) {
	return "\t<tr>\n" .
			"\t\t<td>{$label}</td>\n" .
			"\t\t<td>{$field}</td>\n" .
		"\t</tr>\n";

}

function mwp_print_anchor ( $name ) {
	return "<a name='{$name}'></a>\n";

}

function mwp_print_link ( $caption, $url ) {
	return "<a href='{$url}'>" . __( $caption, 'mangopay_wp_plugin') . "</a>\n";
}


function mwp_print_tag ( $tag, $caption, $key = null ) {
	if ( $key )
		$class = "class='{$key}'";
	return "<{$tag} {$class}>" . __( $caption, 'mangopay_wp_plugin') . "</{$tag}>\n{$description_output}";
}

function mwp_print_table ( $open, $class = 'form-table' ) {
	return  ( $open ? "<table class='{$class}'>\n" : "</table>\n" );
}

function mwp_print_div ( $open, $type = null, $display = true) {
	$display =  ( $display ? 'inline' : 'none');
	return  ( $open ? "<div class='{$type}' style='display:{$display}'>\n"  : "</div>\n" );
}

function mwp_print_button ( $button ) {
	if ( $button ) 
		return mwp_print_row ( "", "<input type='button' url='{$button[1]}' value='" . __($button[0], 'mangopay_wp_plugin') . "' class='{$button[0]}'>");

}

function mwp_print_submit ( $button ) {
	if ( $button ) 
		return mwp_print_row ( "", "<input type='submit' value='" . __($button, 'mangopay_wp_plugin') . "' >");

}

function mwp_print_section ($type, $defaults, $yFields, $display = true, $button = null) {
	
	$output = mwp_print_div ( true, $type, $display ) .
			mwp_print_anchor ( $type ) .
			mwp_print_tag ( "h4", $type ) .
			mwp_print_table ( true );
				foreach ( $yFields as $field ) 
					$output .= mwp_print_inputbox ( $field, ( $defaults ? $defaults -> $field : null ) );
	$output .= 		mwp_print_submit ( $button ) .
			mwp_print_table ( false ) .
	 	mwp_print_div ( false );
	return $output;

}

function mwp_print_section_info ( $type, $defaults, $yFields, $display = true, $button = null, $show_description = true, $extras = null ) {

	$output = mwp_print_div ( true, $type, $display ) .
		//Title
			mwp_print_anchor ( $type ) .
			mwp_print_tag ( "h4", $type ) .
			mwp_print_table ( true );
				foreach ( $yFields as $field )
 					$output .= mwp_print_label ( $field, ( $defaults ? $defaults -> $field : null ), $show_description );
	$output .= $extras .
				mwp_print_submit ( $button ) .
			mwp_print_table ( false ) .
	 	mwp_print_div ( false);
	return $output;

}

?>
