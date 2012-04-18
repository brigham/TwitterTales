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
<title>Editing <?php print $novelname; ?></title>
<link rel='stylesheet' type='text/css' href='style.css'/>
<style type='text/css'>
  .mark_3 { text-decoration: line-through; }
</style>
<script type="text/javascript" src="jquery.js"></script>
<script type="text/javascript">

var curr_hover = 0;
var curr_tweet = '';
var curr_classes = new Array();

$(document).ready(function() {
    $(".tweet").live("click", function(event) {
	$(this).fadeTo(250, 0.4);

	var tweet_id = $(this).attr('id').substr(5);
	var first_para = $(this).hasClass('mark_1');

	jQuery.post("/para.php",
		    { 'tale': '<?php print $novelname; ?>',
			'when': tweet_id,
			'action': 'action-1',
			'activate': ! first_para},
		    '', 'script');		      
      });
    
    $(".tweet").live("mouseover", function(event) {
	var curr_x = event.pageX;
	var curr_y = event.pageY;
	var curr_next_tweet = $(this).attr('id').substr(5);
	var curr_next_classes = $(this).attr('class');
	curr_hover += 1;
	
	var match_value = curr_hover;
	window.setTimeout(function() {
	    if (curr_hover == match_value) {
	      curr_tweet = curr_next_tweet;
	      curr_classes = curr_next_classes;

	      $("#toolbar").css('opacity', 0.0).
		css('left', curr_x + 'px').
		css('top', curr_y + 'px').
		fadeTo(100, 1.0);	      
	    }
	  }, 250);
      });

    $(".toolbarbutton").click(function(event) {
	event.preventDefault();
	var parts = $(this).attr('id').split(/-/);
	var action_val = parts[1];
	var action_type = parts[0];
	
	if (action_type == 'action') {
	  jQuery.post("/para.php",
		      { 'tale': '<?php print $novelname; ?>',
			  'when': curr_tweet,
			  'action': $(this).attr('id'),
			  'activate': curr_classes.indexOf('mark_' + action_val) == -1
			  },		    
		      '', 'script');
	} else if (action_type == 'special') {
	  if (action_val == 1) {
	    $('#new_page_when').get(0).value = curr_tweet;
	    $('#new_page_form').submit();
	  }
	}
      });
  });

function add_mark(action, id) {
  if (action == 1) add_para(id);
  else {
    $('#' + id).addClass('mark_' + action);
  }
}

function del_mark(action, id) {
  if (action == 1) del_para(id);
  else {
    $('#' + id).removeClass('mark_' + action);
  }
}

function del_para(id) {
  var hashid = '#' + id;

  var parentDiv = $(hashid).parent().get(0);
  var newHomeDiv = $(hashid).parent().prev().get(0);
  $(parentDiv).children().remove().appendTo(newHomeDiv);
  $(parentDiv).remove();
  $(hashid).removeClass('mark_1');
  $(hashid).fadeTo(250, 1.0);
}

function add_para(id) {
  var hashid = '#' + id;

  var parentDiv = $(hashid).parent().get(0);
  var newDiv = $("<div/>");
  $(parentDiv).after(newDiv);
  $(hashid).nextAll().remove().appendTo(newDiv);
  $(hashid).remove().prependTo(newDiv);
  $(hashid).addClass('mark_1');
  $(hashid).fadeTo(250, 1.0);
}

</script>
</head>
<body>
<?php
render_tweets_for_novel($novelname, $page, 0, true);
?>

<div id="toolbar" style='position: absolute; opacity: 0'>
  <a href='#' class='toolbarbutton' id='action-3'><img src='delete.png'/></a>
  <a href='#' class='toolbarbutton' id='special-1'><img src='section.png'/></a>
</div>

<div id="new_page" style='visibility: hidden' >
  <form id='new_page_form' action='/newpage.php' method='post'>
    <input type='hidden' id='new_page_tale' name='tale' value='<?php print $novelname; ?>'/>
    <input type='hidden' id='new_page_page' name='page' value='<?php print $page+1; ?>'/>
    <input type='hidden' id='new_page_when' name='when' value=''/>
  </form>
</div>
</body>
</html>
