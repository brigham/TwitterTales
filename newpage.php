<?php

require_once('../protected/utils.inc');
require_once('../protected/render.inc');

get_db();

$novelname = $_POST['tale'];
$when = $_POST['when'];
$page = $_POST['page'];

if (! isset($novelname) || ! verify_id($novelname) ) {
  exit();
}

if (! isset($when) || ! verify_date($when) ) {
  exit();
}

if (! isset($page) || ! verify_number($page) ) {
  exit();
}

add_page_for_tweet($novelname, $when, $page);

header("Location: http://{$_SERVER['HTTP_HOST']}/edit.php?tale=$novelname&page=$page");

?>