<?php

require_once('utils.inc');

function render_tweets_for_novel($novelname, $page, $format, $edit) {

  if (! verify_id($novelname) ) return ;
  if (! verify_number($page)  ) return ;

  $novel_info = get_novel_data($novelname);

  $page_range = get_start_and_end_for_page($novelname, $page);

  $start = $page_range['start'];
  $end = $page_range['end'];

  $tweets = get_tweets_for_novel($novelname, $start, $end);

  $marks = get_marks_for_range($novelname, $start, $end);

  $last_page = get_max_page_for_novel($novelname);

  if ($page == 1) {
    print "<h1>{$novel_info['humanname']}</h1>";
    $user = $novel_info['twittername'];
    print "<h2>by @<a href='http://twitter.com/$user'>$user</a></h2>";
  }

  print "<div class='tweetpara'>";

  foreach ($tweets as $tweet) {
    $text = str_replace("#{$novel_info['hashtag']}", '', $tweet['text']);

    $when = $tweet['when'];
    if (isset($marks[$when]) && isset($marks[$when][1])) {
      print "</div><div class='tweetpara'>";
      if (isset($marks[$when][2])) {
	print "***</div><div class='tweetpara'>";
      }
    }

    if ($edit || ! isset($marks[$when]) || ! isset($marks[$when][3])) {
      print "<span class='tweet ";
      if (isset($marks[$when])) {
	foreach ($marks[$when] as $key=>$value) {
	  print "mark_$key ";
	}
      }
      print "' id='tweet$when'>";

      print htmlentities($text) . "</span>";
    }

  }

  print "</div>";

  print "<p>";

  if ($page > 1) {
    $nextpage = $page - 1;
    page_link($novelname, $nextpage, "&lt; Previous Page", $edit);
  }
  
  if (isset($end)) {
    $nextpage = $page + 1;
    page_link($novelname, $nextpage, "Next page &gt;", $edit);
  }

  print "</p>";

  if ($last_page != 0) {

    print "<p>";
    for ($i = 1; $i <= $last_page; $i++) {
      if ($page != $i) {
	page_link($novelname, $i, "$i", $edit);
      } else {
	print "$i ";
      }
    }
    print "<p>";

  }

}

function page_link($novelname, $page, $text, $edit) {
    if (! $edit) {
      if ($page != 1) {
	print "<a href='/{$novelname}/{$page}'>$text</a> ";
      } else {
	print "<a href='/{$novelname}'>$text</a> ";
      }
    } else {
      print "<a href='/edit.php?tale=$novelname&page=$page'>$text</a> ";
    }
}

?>