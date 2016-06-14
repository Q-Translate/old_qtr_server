<?php
$path = dirname(dirname(__FILE__));
include_once($path . '/config.php');
include_once($path . '/http_request.php');

function link_to($url) {
    print("<li>Click: <a href='$url' target='_blank'>$url</a></li>");
};

print "<br/><hr/><br/>\n";

// Get a RSS feed of the latest translations.
link_to('https://l10n.org.al/qtr/rss-feed');
link_to('https://l10n.org.al/qtr/rss-feed/LibreOffice');
link_to('https://l10n.org.al/qtr/rss-feed/LibreOffice/cui');

link_to('https://qtranslate.net/qtr/rss-feed/LibreOffice/cui/sq');
link_to('https://qtranslate.net/qtr/rss-feed/LibreOffice/cui');

link_to('https://qtranslate.org/rss-feed/sq');
link_to('https://qtranslate.org/rss-feed/sq/LibreOffice');
link_to('https://qtranslate.org/rss-feed/sq/LibreOffice/cui');
