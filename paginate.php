<?php
require_once('../protected/utils.inc');

get_db();

$novelname = $_GET['tale'];

if (! isset($novelname) || ! verify_id($novelname) ) {
  exit();
}




?>