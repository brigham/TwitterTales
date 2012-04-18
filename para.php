<?php

require_once('../protected/utils.inc');
require_once('../protected/render.inc');

get_db();

$novelname = $_POST['tale'];
$when = $_POST['when'];
$action = $_POST['action'];
$activate = $_POST['activate'];

if (! isset($novelname) || ! verify_id($novelname) ) {
  print '// bad novel name';
  exit();
}

if (! isset($when) || ! verify_date($when) ) {
  print '// bad tweet id';
  exit();
}

if (! isset($action) || (substr($action, 0, 7) != 'action-')) {
  print "// bad action: $action";
  exit();
}

$action = substr($action, 7);

if ($action < 1 || $action > 3) {
  print '// action out of range';
  exit();
}

if (! isset($activate) || ($activate != 'true' && $activate != 'false')) {
  print '// no command';
  exit();
}

if ($activate == 'true') {
  if ( add_mark_for_tweet($novelname, $when, $action) ) {
    print "add_mark($action, 'tweet$when');";
  } else {
    print '// could not add mark';
  }
} else if ($activate == 'false') {
  if ( del_mark_for_tweet($novelname, $when, $action) ) {
    print "del_mark($action, 'tweet$when');";
  } else {
    print '// could not remove mark';
  }
} else {
  print '// bad action';
}

?>