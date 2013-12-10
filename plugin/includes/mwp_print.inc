<?php

function mwp_print_checkbox ( $name, $is_checked ) {
	$checked = ( $is_checked  ? " checked='checked' " : null );
	return mwp_print_row (
			mwp_print_tag ( 'label', $name, false ),
			"<input name='{$name}' type='checkbox' {$checked} class='{$name}' />".
			"<span class='description'>" . __( "{$name}_description", 'mangopay_wp_plugin') . "</span>"
	);

}

function mwp_print_label ( $key, $value ) {
	return mwp_print_row (
		"<b>" . mwp_print_tag ('label', $key, $false) . "</b>",
		mwp_print_tag ('label', $value, $false)
	);
}

function mwp_print_inputbox ( $key, $value ) {
	return mwp_print_row (
		mwp_print_tag ('label', $key, $false) ,
		"<input type='text' id='mwp_{$key}' name='mwp_{$key}' value='{$value}' class='regular-text'>&nbsp;" .
		"<span class='description'>" . __( "{$key}_description", 'mangopay_wp_plugin') . "</span>\n"
	);

}

function mwp_print_row ( $label, $field ) {
	return "\t<tr>\n" .
			"\t\t<th>{$label}</th>\n" .
			"\t\t<td>{$field}</td>\n" .
		"\t</tr>\n";

}

function mwp_print_anchor ( $name ) {
	return "<a name='{$name}'></a>\n";

}

function mwp_print_tag ( $tag, $caption, $echo = true ) {
	return "<{$tag}>" . __( $caption, 'mangopay_wp_plugin') . "</{$tag}>\n";

}

function mwp_print_table ( $open ) {
	return  ( $open ? "<table class='form-table'>\n" : "</table>\n" );

}

function mwp_print_button ( $button ) {
	if ( $button ) 
		return mwp_print_row ( "", "<input type='button' url='{$button[1]}' value='{$button[0]}' class='{$button[0]}'>");

}

function mwp_print_submit ( $button ) {
	if ( $button ) 
		return mwp_print_row ( "", "<input type='submit' value='{$button}' >");

}

function mwp_print_section ($type, $defaults, $yFields, $display = true, $button = null) {
	$display =  ( $display ? 'inline' : 'none');
	$output = "<div class='{$type}' style='display:{$display}'>\n" .
		//Title
			mwp_print_anchor ( $type ) .
			mwp_print_tag ( "h4", $type ) .
			mwp_print_table ( true );
				foreach ( $yFields as $field ) 
					$output .= mwp_print_inputbox ( $field, ( $defaults ? $defaults -> $field : null ) );
	$output .= 	mwp_print_submit ( $button ) .
			mwp_print_table ( false ) .
	 	"</div>\n";
	return $output;

}

function mwp_print_section_info ( $type, $defaults, $yFields, $display = true ) {
	$display =  ( $display ? 'inline' : 'none');
	$output = "<div class='{$type}' style='display:{$display}'>\n" .
		//Title
			mwp_print_anchor ( $type ) .
			mwp_print_tag ( "h4", $type ) .
			mwp_print_table ( true );
				foreach ( $yFields as $field )
 					$output .= mwp_print_label ( $field, ( $defaults ? $defaults -> $field : null ) );					
	$output .=	mwp_print_table ( false ) .
	 	"</div>\n";
	return $output;

}

?>
