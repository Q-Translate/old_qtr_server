<?php
/**
 * @file
 * Functions that are used for sending notification emails.
 */

/**
 * Sends by email a notification message.
 *
 * The subject and body of the message depend on the $params->type.
 * Other required attributes of $params are $params->recipient
 * and $params->uid. The other attributes are needed to build the
 * subject and body of the message. Some common attributes are:
 *   $params->username, $params->sguid, $params->string,
 *   $params->translation, etc.
 */
function btrCore_send_notification_by_email($params) {
  // See: http://api.drupal.org/api/drupal/includes%21mail.inc/function/drupal_mail/7
  $account = user_load($params->uid);
  drupal_mail(
    $module   = 'btrCore',
    $key      = 'notifications',
    $to       = $params->recipient,
    $langcode = $account->language,
    $params   = _btrCore_build_notification_email($params),
    $from     = _btrCore_get_email_sender(),
    $send     = TRUE
  );
}


/**
 * Return the sender of the email notifications
 * (which is always the same, as defined on smtp variables)
 */
function _btrCore_get_email_sender() {
  $smtp_from = variable_get('smtp_from');
  $smtp_fromname = variable_get('smtp_fromname');
  $sender = "$smtp_fromname <$smtp_from>";
}


/**
 * Returns the subject and body of a notification email:
 *    array('subject' => $subject, 'body' => $body)
 *
 * The subject and body of the message depend on the type
 * of the notification, defined by $params->type.
 *
 * The other attributes depend on the notification type.
 * Some common attributes are: $params->uid, $params->sguid,
 * $params->username, $params->string, $params->translation, etc.
 */
function _btrCore_build_notification_email($params) {

  $account = user_load($params->uid);
  $lng = $account->translation_lng;
  $subject_prefix = "l10n-$lng";

  // Get the base_url of the translation site.
  module_load_include('inc', 'btrCore', 'includes/sites');
  $base_url = btr_get_base_url($lng);

  if (isset($params->sguid)) {
    $url = $base_url . "/translations/$lng/" . $params->sguid;
  }

  switch ($params->type) {

    // Send an email notification to a user, when a translation that he
    // has voted for is modified.
    case 'notify-voter-on-translation-change':
      $subject = t("[!prefix] Translation modified for: !string",
                 array(
                   '!prefix' => $subject_prefix,
                   '!string' => btrCore_shorten($params->string, 30),
                 ));
      $body = array(
        t("Greetings !username,

           The following translation has been modified by its author.
           You are being notified because you have voted for the old
           translation and your vote now counts for the new version
           of the translation.

           If the new version of the translation is OK for you,
           then there is nothing you should do. Otherwise, you
           can change your vote at this link: !url

           Thanks

           ---------- Original (English) String:
           !string
           ---------- Old Translation:
           !old_translation
           ---------- New Translation:
           !new_translation
           ----------
           ",
          array(
            '!username' => $params->username,
            '!url' => $url,
            '!string' => $params->string,
            '!old_translation' => $params->old_translation,
            '!new_translation' => $params->new_translation,
          )),
      );
      break;

    // Notify the previous voters of a string that a new translation has been
    // submitted. Maybe they would like to review it and change their vote.
    case 'notify-voter-on-new-translation':
      $subject = t("[!prefix] New translation for: !string",
                 array(
                   '!prefix' => $subject_prefix,
                   '!string' => btrCore_shorten($params->string, 30),
                 ));
      $body = array(
        t("Greetings !username,

           A new translation has been suggested for a string
           that you have voted for. If you think that the new
           translation is more suitable than the one that you
           have voted for, then you can change your vote
           at this link: !url

           Thanks

           ---------- Original (English) String:
           !string
           ---------- Voted Translation:
           !voted_translation
           ---------- New Translation:
           !new_translation
           ----------
          ",
          array(
            '!username' => $params->username,
            '!url' => $url,
            '!string' => $params->string,
            '!voted_translation' => $params->voted_translation,
            '!new_translation' => $params->new_translation,
          )),
      );
      break;

    // Notify translators that a new term has been added to the vocabulary.
    case 'notify-on-new-vocabulary-term':
      $app_url = btr_get_app_url($lng, 'vocabulary', $params->project);
      $subject = t("[!prefix] New term: !string",
                 array(
                   '!prefix' => $params->project,
                   '!string' => $params->string,
                 ));
      $body = array(
        t("Greetings !username,

           A new term has been added to the vocabulary !project by !author.
           Check it out at the mobile app: !url

           Thanks
          ",
          array(
            '!username' => $params->username,
            '!project' => $params->project,
            '!author' => $params->author,
            '!string' => $params->string,
            '!url' => $app_url . '/#' . $params->string,
          )),
      );
      break;

    // Notify voters that a new comment has been added.
    case 'notify-on-new-disqus-comment':
      $subject = t("[!prefix] !title",
                 array(
                   '!prefix' => $subject_prefix,
                   '!title' => $params->title,
                 ));
      $body = array(
        t("Greetings !username,

           A new comment has been added to a string that you have voted.
           !link

           -----
           !comment
           -----

           Thanks
          ",
          array(
            '!username' => $params->username,
            '!link' => $params->link,
            '!comment' => $params->comment,
          )),
      );
      break;

    // Notify the author of a translation, when it is deleted.
    case 'notify-author-on-translation-deletion':
      $subject = t("[!prefix] Translation deleted: !translation",
                 array(
                   '!prefix' => $subject_prefix,
                   '!translation' => btrCore_shorten($params->translation, 30),
                 ));
      $body = array(
        t("Greetings !username,

           A translation that you have submitted, has been deleted by a moderator.
           You can see the string and the rest of translations at this link: !url

           Thanks

           ---------- Original (English) String:
           !string
           ---------- Deleted Translation:
           !translation
           ----------
           ",
          array(
            '!username' => $params->username,
            '!url' => $url,
            '!string' => $params->string,
            '!translation' => $params->translation,
          )),
      );
      break;

    // Notify the voters of a translation, when it is deleted.
    case 'notify-voter-on-translation-deletion':
      $subject = t("[!prefix] Translation deleted: !translation",
                 array(
                   '!prefix' => $subject_prefix,
                   '!translation' => btrCore_shorten($params->translation, 30),
                 ));
      $body = array(
        t("Greetings !username,

           A translation that you have voted, has been deleted by a moderator.
           You can see the string and the rest of translations at this link: !url

           Thanks

           ---------- Original (English) String:
           !string
           ---------- Deleted Translation:
           !translation
           ----------
           ",
          array(
            '!username' => $params->username,
            '!url' => $url,
            '!string' => $params->string,
            '!translation' => $params->translation,
          )),
      );
      break;

    // Notify admin about an export request.
    case 'notify-admin-on-export-request':
      $subject = t("[!prefix] Export request: !origin/!project",
                 array(
                   '!prefix' => $subject_prefix,
                   '!project' => $params->project,
                   '!origin' => $params->origin,
                 ));
      $body = array(
        t("Export request by user !uid for '!origin/!project'.",
          array(
            '!uid' => $params->uid,
            '!project' => $params->project,
            '!origin' => $params->origin,
          )),
      );
      break;

    // Notify admin about an import request.
    case 'notify-admin-on-import-request':
      $subject = t("[!prefix] Import request",
                 array('!prefix' => $subject_prefix));
      $body = array(
        t("Import request by user !username (!uid): !filename.",
          array(
            '!username' => $params->username,
            '!uid' => $params->uid,
            '!filename' => $params->filename,
          )),
      );
      break;

    // Notify the user that the export is ready for download.
    case 'notify-that-export-is-done':
      $subject = t("[!prefix] Export: !project",
                 array(
                   '!prefix' => $subject_prefix,
                   '!project' => $params->project,
                 ));
      $body = array(
        t("Greetings !username,

           The export of '!project', that you have requested, is done.
           You can download it from these links:
           * !export_url_tgz
           * !export_url_diff
           * !export_url_ediff
           ",
          array(
            '!username' => $params->username,
            '!project' => $params->project,
            '!export_url_tgz' => $params->export_url_tgz,
            '!export_url_diff' => $params->export_url_diff,
            '!export_url_ediff' => $params->export_url_ediff,
          )),
      );
      break;

    // Notify the user that the project import is done.
    case 'notify-that-project-import-is-done':
      $subject = t("[!prefix] Import of project !project",
                 array(
                   '!prefix' => $subject_prefix,
                   '!project' => $params->project,
                 ));
      $body = array(
        t("Greetings !username,

           The import of the project '!project', that you have requested,
           is done. Check it out here:
           !search_url
           ",
          array(
            '!username' => $params->username,
            '!project' => $params->project,
            '!search_url' => $params->search_url,
          )),
      );
      break;

    // Notify the user that the import is done.
    case 'notify-that-import-is-done':
      $subject = t("[!prefix] Import of !filename",
                 array(
                   '!prefix' => $subject_prefix,
                   '!filename' => $params->filename,
                 ));
      $body = array(
        t("Greetings !username,

           The import of '!filename', that you have requested, is done.
           Check out the new translations here:
           !search_url
           ",
          array(
            '!username' => $params->username,
            '!filename' => $params->filename,
            '!search_url' => $params->search_url,
          )),
      );
      if ($params->messages) {
        $body .= "\n\n" . t('Output messages:') . "\n" . $params->messages;
      }
      break;

    // Send by email a string to be reviewed.
    case 'string-to-be-reviewed':
      list($string, $translations) = btr::string_get_translations($params->sguid, $lng);
      // Get the url of the string.
      list($origin, $project) = explode('/', $params->project);
      $app_url = btr_get_app_url($lng, $origin, $project);
      if ($app_url and $origin=='vocabulary') {
        $url = "$app_url?lng=$lng&proj=$project#$string";
      }

      $subject = "[$subject_prefix] " . btrCore_shorten($string, 60);
      $body = array(
        t("Greetings !username,

       If you have time, please help to review the translation
       of the string at this link: !url

       Thanks

       ---------- Original (English) String:
       !string
       ---------- Available Translations:
      ",
          array(
            '!username' => $params->username,
            '!url' => $url,
            '!string' => $string,
          )),
      );
      foreach ($translations as $translation) {
        $body[] = ' * ' . $translation;
      }
      break;
  }

  return array(
    'subject' => $subject,
    'body' => $body,
  );
}