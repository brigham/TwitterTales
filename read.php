<?php
require_once('../protected/utils.inc');
require_once('../protected/render.inc');

get_db();

$novelname = $_GET['tale'];
$page = $_GET['page'];

if (! isset($novelname) || ! verify_id($novelname) ) {
  $novelname = 'FuelDump';
}

if (! isset($page) || ! verify_number($page) ) {
  $page = '1';
}
?>

<html>
<head>
<title><?php print $novelname; ?></title>
<link rel='stylesheet' type='text/css' href='/style.css'/>
</head>
<body>
<?php
  render_tweets_for_novel($novelname, $page, 0, false);
?>
</body>
</html>