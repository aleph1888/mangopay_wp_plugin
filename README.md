mangopay_wp_plugin
==================

Mangopay plugin for wordpress.

Descripción 
--------------
Simple plugin para wordpress. Pasarela de pago Mangopay. Apartir del SDKv2 (https://github.com/MangoPay/mangopay2-php-sdk)

Acciones por ROL de USUARIO:
--------------
**Usuario >= author**
- Colocar *shortcodes de contribución* relacionados con posts. Se crea una *wallet* en Mangopay por cada post con *shortcode*; siendo el autor del post el *usuario propietario* de la cartera virtual.
- Rellenar campos adicionales en profile, *beneficiary*; el *usuario propietario* de la cartera debe rellenar estos campos para poder liquidar la recolecta.
- Solicitar liquidación de lo recolectado por un post.

**Usuario <= contributor**
- Rellenar campos adicionales en profile, *contributor*.
- Contribuir mediante llamadas al *shortcode contribución*.

Shortcodes:
--------------
**Shortcode contribución**: [mwp_contribute amount="999" post_id="88"] 

![alt tag](https://raw.github.com/aleph1888/mangopay_wp_plugin/master/images/contribute_shortcode_0.jpg)

![alt tag](https://raw.github.com/aleph1888/mangopay_wp_plugin/master/images/contribute_shortcode_1.jpg)

- Vista: Botón de contribuir; input text cantidad.
- Parámetros:
	* Post_id => Si ausente, el contenedor del shortcode.
	* Amount => Si ausente, se muestra un inputbox.

**Shortcode recogido**: [mwp_raised post_id="88"] 

![alt tag](https://raw.github.com/aleph1888/mangopay_wp_plugin/master/images/raised_shortcode.jpg)

- Vista: Literal informando del total recogido por el proyecto. 
- Parámetros:
	* Post_id => Si ausente, el contenedor del shortcode.

Acciones:
--------------
- Rellenar campos adicionales en profile, *contributor*.
- Rellenar campos adicionales en profile, *beneficiary*.

![alt tag](https://raw.github.com/aleph1888/mangopay_wp_plugin/master/images/Profile-fields.jpg)

- Solicitar liquidación de lo recolectado por un post.

![alt tag](https://raw.github.com/aleph1888/mangopay_wp_plugin/master/images/Post-fields.jpg)

Internacionalización: 
--------------
/languages/

Instalación
--------------
- Requisitos para el SDKv2 de Mangoplay
	* Curl
	* Openssl
- Consigue la *frase de paso* de tu plataforma en Mangopay configurando y ejecutando *./client_creation.php*
- Establece las credenciales en tu fichero de configuración Wordpress, *wp-config.php*.
```
    /** Mangopay info  */
    define('MWP_client_id', '<ClientID>');
    define('MWP_password', '<password>');
    define('MWP_temp_path', '<temp_path>');
    define('MWP_base_path', 'https://api.sandbox.mangopay.com'); //o 'https://api.mangopay.com' para producción
```

Contribuye
--------------
@BTC 1DNxbBeExzv7JvXgL6Up5BSUvuY4gE8q4A


Contri
