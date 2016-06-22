<?php
$path = dirname(dirname(__FILE__));
include_once($path . '/config.php');
include_once($path . '/http_request.php');

function link_to($url) {
    print("<li>Click: <a href='$url' target='_blank'>$url</a></li>");
};

print "<br/><hr/><br/>\n";

// Get a random tweet.
link_to('https://sq.qtranslate.net/qtr/tweet');
link_to('https://sq.qtranslate.net/qtr/tweet/vocabulary/ICT_sq');

link_to('https://qtranslate.net/qtr/tweet/vocabulary/ICT_sq/sq');

link_to('https://qtranslate.org/tweet/sq');
link_to('https://qtranslate.org/tweet/sq/vocabulary/ICT_sq');

http_request('https://qtranslate.org/tweet/sq',
  ['headers' => ['Accept' => 'application/json']]);