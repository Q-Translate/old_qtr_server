<?php
/**
 * @file
 * Resource definition and callbacks for report.
 */

function _btrServices_report() {
  $file = array('type' => 'inc', 'module' => 'btrServices', 'name' => 'resources/report');
  $access = array(
    'access callback' => 'user_access',
    'access arguments' => array('access content'),
  );

  // Getting reports of different kinds.
  $report['operations']['retrieve'] = array(
    'help' => 'Reports of different kinds.',
    'file' => $file,
    'callback' => '_btr_report_retrieve',
    'args' => array(
      array(
        'name' => 'report',
        'optional' => FALSE,
        'source' => array('path' => 0),
        'description' => 'The name/id of the report.',
        'type' => 'string',
      ),
      array(
        'name' => 'params',
        'optional' => FALSE,
        'source' => 'param',
        'description' => 'Parameters of the report.',
        'type' => 'array',
      ),
    ),
  ) + $access;

  // Getting general statistics (number of votes and
  // translations) for the last week, month and year.
  $report['actions']['statistics'] = array(
    'help' => 'General statistics (number of votes and translations) for the last week, month and year.',
    'file' => $file,
    'callback' => '_btr_report_statistics',
    'args' => array(
      array(
        'name' => 'params',
        'optional' => FALSE,
        'source' => 'data',
        'description' => 'The parameters of the report.',
        'type' => 'array',
      ),
    ),
  ) + $access;

  // Getting statistics about top contributors.
  $report['actions']['topcontrib'] = array(
    'help' => 'Statistics about top contributors.',
    'file' => $file,
    'callback' => '_btr_report_topcontrib',
    'args' => array(
      array(
        'name' => 'params',
        'optional' => FALSE,
        'source' => 'data',
        'description' => 'The parameters of the report.',
        'type' => 'array',
      ),
    ),
  ) + $access;

  return $report;
}

/**
 * Getting reports of different kinds. Anybody can
 * retrieve them, there are no access restrictions.
 *
 * @param $report
 *   The name/id of the report.
 *
 * @param $params
 *   Parameters of the report, comming from the
 *   GET request. They depend on the kind of report.
 *
 * @return
 *   The data of the requested report.
 *
 * @example
 *   curl -k -i \
 *     'https://dev.l10n.org.xx/public/btr/report/statistics?lng=sq'
 *   curl -k -i \
 *     'https://dev.l10n.org.xx/public/btr/report/topcontrib?lng=sq&period=week'
 */
function _btr_report_retrieve($report, $params) {
  $report_function = '_btr_report_' . $report;
  if (! function_exists($report_function) ) {
    return services_error("Unknown report: $report", 406);
  }
  return $report_function($params);
}

/**
 * Getting general statistics (number of votes and
 * translations) for the last week, month and year.
 *
 * @param $params
 *   Parameters of the report, comming from the POST request.
 *   It contains these fields (variables):
 *   - lng: language of translations
 *
 * @return
 *   Array of general stats for week, month and year.
 *
 * @example
 *   curl -k -i -H "Content-type: application/json"  \
 *        -X POST https://dev.l10n.org.xx/public/btr/report/statistics \
 *        -d '{"lng": "sq"}'
 */
function _btr_report_statistics($params) {
  // get parameters and validate them
  $lng = $params['lng'];
  $arr_langs = array_keys(btr_get_languages());
  if (!in_array($lng, $arr_langs)) {
    return services_error('Language missing or not supported.', 406);
  }

  return btr_statistics($lng);
}

/**
 * Getting statistics about top contributors.
 *
 * @param $params
 *   Parameters of the report, comming from the POST request.
 *   It contains these fields (variables):
 *   - lng: language of translations
 *   - period: period of report (last day|week|month|year)
 *   - size: number of results (top contributers), between 5 and 20
 *
 * @return
 *   Array of users, where each user is an object with these attributes:
 *   uid, name, umail, score, translations, votes
 *
 * @example
 *   curl -k -i -H "Content-type: application/json"  \
 *     -X POST https://dev.l10n.org.xx/public/btr/report/topcontrib \
 *     -d '{"lng": "sq", "period": "week", "size": 10}'
 */
function _btr_report_topcontrib($params) {
  // get parameters and validate them
  $lng = $params['lng'];
  $arr_langs = array_keys(btr_get_languages());
  if (!in_array($lng, $arr_langs)) {
    return services_error('Language missing or not supported.', 406);
  }
  $period = $params['period'];
  if (!in_array($period, array('day', 'week', 'month', 'year'))) {
    return services_error('Period missing or not supported.', 406);
  }
  $size = (int) $params['size'];
  if ($size < 5)  $size = 5;
  if ($size > 20) $size = 20;

  return btr_stat_topcontrib($lng, $period, $size);
}