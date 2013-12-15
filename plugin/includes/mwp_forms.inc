<?php
namespace mwp;
	require_once ( __DIR__ . '/mwp_print.inc');

	class mwp_forms {

		static private function f_user_section_header ( $defaults, $display = true ) {
			return mwp_print_div ( true, 'mangopay_userheader', $display ) . 
				mwp_print_tag ( "h3", 'mangopay_user_info' ) .
				mwp_print_table ( true ) .
					mwp_print_checkbox ( 'user_type', $defaults -> is_legal_user) .
				mwp_print_table ( false ) .
				mwp_print_div ( false);

		}

		static function f_ini ( $css_js = false ) {
			if (  $css_js ) {
				wp_enqueue_style( 'mwp_sc_contribute_css' );
				wp_enqueue_script( 'mwp_sc_contribute_js' );
			}
		}
	
		static function mwp_show_user_section ( $defaults, $display = true ) {
			mwp_forms::f_ini  ( true );

			$output .= mwp_print_tag ( "label", "[{$defaults -> mangopay_id}]" );
			$output .= mwp_print_tag ( "label", "[{$defaults -> is_legal_user}]" );
			$output .= mwp_print_tag ( "label", "[{$defaults -> bank_id}]" );
			$output .= mwp_print_tag ( "label", "[{$defaults -> card_id}]" ) . "<br>\n";

			if ( ! $display )
				$output .= mwp_print_button ( array('bt_change_user_data') );

			$output .= mwp_forms::f_user_section_header ( $defaults, $display );
	
			$display_ = $defaults -> is_legal_user && $display;
			$yFields = mwp_get_fields ( 'Legal' );
			$output .= mwp_print_section("mangopay_legal", $defaults, $yFields, $display_);

			$display_ = ( ! $defaults -> is_legal_user ) && $display;
			$yFields = mwp_get_fields ( 'Natural' );
			$output .= mwp_print_section("mangopay_natural", $defaults, $yFields, $display_);
			
			return $output;

		}

		static function mwp_show_payment_section ( $defaults, $display ) {
			mwp_forms::f_ini ();

			$yFields = mwp_get_fields ( 'register' );
			$output  .= mwp_print_section("mangopay_cards", $defaults, $yFields, $display);
			if ( ! $display )
				$output .= mwp_print_button ( array ('bt_register_card') );

			$extras = mwp_print_inputbox ( 'Amount', $defaults->Amount );
			$display = true; $desc = true;
			$yFields = mwp_get_fields ( 'pay' );
			$output  .= mwp_print_section_info("mangopay_pay", $defaults, $yFields, $display, 'bt_pay', $desc, $extras);
			 
			return $output;

		}

		static function mwp_show_bank_section ( $defaults ) {
			mwp_forms::f_ini ();
			$can_payout =  $defaults-> roles[0] != "suscriber" && $defaults-> roles[0] != "contributor";
			
			$yFields = mwp_get_fields ( 'bank' );
			$output  .= mwp_print_section("mangopay_bank", $defaults, $yFields, $can_payout );

			/*//registered banks
			require_once ( __DIR__ . "/mwp_bank.inc");
			$output  .= mwp_print_list ("mangopay_bank",  $defaults );*/

			return $output;

		}

		static function mwp_show_bank_section_info ( $defaults ) {
			mwp_forms::f_ini ();
			$yFields = mwp_get_fields ( 'bank' );
			$display = true; $desc = false;
			return mwp_print_section_info("mangopay_bank", $defaults, $yFields, $display, null, $desc );
		}

		static function mwp_show_wordpress_login() {
			if ( ! is_user_logged_in() ) {
				return mwp_print_link ( "login", wp_login_url() ) . "<br>" .
					mwp_print_link ( "register", wp_registration_url() );
			}
		}
	}
?>
