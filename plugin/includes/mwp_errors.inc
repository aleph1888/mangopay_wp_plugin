<?php 
namespace mwp;

	class mwp_errors {
		
		static public function mwp_manage_api_error ( $e, $details, $source ) {

			if ( $details ) {
				foreach ($e->GetErrorDetails() -> Errors as $error)
					$output .= $error . " ";

				$_SESSION["MWP_API_ERROR"] .= "[{$source}] {$output}<br><b>\n"; 
			} else {
				$_SESSION["MWP_API_ERROR"] .= "[{$source}] {$e->GetMessage()}<br><b>\n";
			}
			return;
		}


	}

?>
