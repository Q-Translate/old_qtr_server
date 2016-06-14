<?php
$path = dirname(dirname(__FILE__));
include_once($path . '/config.php');
include_once($path . '/http_request.php');

function download($url) {
    print("<li>Download: <a href='$url'>$url</a></li>\n");
};

print "<br/></br/>";

// Download as a text list (in different formats).
download('https://qtranslator.org/vocabulary/export/ICT_sq/txt1');
download('https://qtranslator.org/vocabulary/export/ICT_sq/txt2');
download('https://qtranslator.org/vocabulary/export/ICT_sq/org');

// Get in JSON format.
http_request('https://qtranslator.org/vocabulary/export/ICT_sq');
