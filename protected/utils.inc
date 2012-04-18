<?php

function get_db() {
  if (! @mysql_connect("HOST", "USER", "PASSWORD") ) {
    echo( "<p>error 1</p>" );
    exit();
  }
  if (! @mysql_select_db("ttales") ) {
    echo( "<p>error 2</p>" );
    exit();
  }
}

function verify_id($id) {
  for ($i = 0; $i < strlen($id); $i++) {
    $ch = $id[$i];
    if (! (($ch >= 'A' && $ch <= 'Z') ||
	   ($ch >= 'a' && $ch <= 'z') ||
	   ($ch == '_'))) {
      return 0;
    }
  }  
  return 1;
}

function verify_date($when) {
  return verify_number($when);
}

function verify_date2($when) {
  if (strlen($when) == 19 &&
      verify_number(substr($when, 0, 4)) &&
      $when[4] == '-' &&
      verify_number(substr($when, 5, 2)) &&
      $when[7] == '-' &&
      verify_number(substr($when, 8, 2)) &&
      $when[10] == ' ' &&
      verify_number(substr($when, 11, 2)) &&
      $when[13] == ':' &&
      verify_number(substr($when, 14, 2)) &&
      $when[16] == ':' &&
      verify_number(substr($when, 17, 2))) return true;
  else return false;
}

function verify_number($num) {
  for ($i = 0; $i < strlen($num); $i++) {
    $ch = $num[$i];
    if (! (($ch >= '0' && $ch <= '9'))) {
      return 0;
    }
  }
  return 1;
}

function get_tweets_for_novel($novelname, $start, $end) {

  if (! verify_id($novelname) ) return ;

  $sql = 
    "select `text`, date_format(`when`, '%Y%m%d%H%i%s') `when` " .
    "from `tweets` " .
    "where `novelname`='$novelname' " .
    (isset($start) ?
     "  and `when`     >='$start'" : "") .
    (isset($end) ?
     "  and `when`     < '$end'"   : "") .
    "order by `when` asc";

  $result = mysql_query($sql);

  if (! $result ) {
    echo( "<p>error 3</p>" );
    exit();
  }

  $reply = array();
  while ($row = mysql_fetch_array($result) ) {
    $tweet = array();
    $tweet['text'] = $row['text'];
    $tweet['when'] = $row['when'];
    $reply[] = $tweet;
  }
  
  return $reply;

}

function get_start_and_end_for_page($novelname, $page) {
  
  if (! verify_id($novelname) ) return array();
  if (! verify_number($page)  ) return array();

  $sql = 
    "select `when` " .
    "from `pages` " .
    "where `novelname` ='$novelname' " .
    "  and `page` between $page and $page+1 " .
    "order by `page`";

  $result = mysql_query($sql);

  if (! $result ) {
    echo ( "<p>error 9</p>" );
    exit();
  }

  $start = '00000000000000';
  if ($row = mysql_fetch_array($result) ) {
    $start = $row['when'];
  }

  $end = '00000000000000';
  if ($row = mysql_fetch_array($result) ) {
    $end = $row['when'];
  }

  $reply = array();

  if ($start != '00000000000000') 
    $reply['start'] = $start;
  if ($end   != '00000000000000')
    $reply['end']   = $end;

  return $reply;

}

function get_marks_for_range($novelname, $start, $end) {
  if (! verify_id($novelname) ) return array();

  $sql =
    "select date_format(`when`, '%Y%m%d%H%i%s') `when`, `marktypeid` " .
    "from `marks` " .
    "where `novelname`='$novelname' " .
    (isset( $start ) ? 
     "  and `when`     >='$start'" : "") .
    (isset( $end ) ?
     "  and `when`     < '$end'"   : "");

  $result = mysql_query($sql);

  if (! $result ) {
    echo ( "<p>error 10</p>" );
    exit();
  }

  $reply = array();

  while ($row = mysql_fetch_array($result) ) {
    $when = $row['when'];
    if (! isset($reply[$when]) ) {
      $reply[$when] = array();
    }
    $reply[$when][$row['marktypeid']] = 1;
  }  

  return $reply;
}

function update_novel_lastid($novelname, $lastid) {
  if (! verify_id($novelname) ) return false;

  $sql = 
    "update `tales` " .
    "set `lastid`=$lastid ".
    "where `novelname`='$novelname'";

  if (mysql_query($sql) ) {
    print "<p>updated $novelname lastid to $lastid</p>";
  } else {
    print "<p>error 8</p>";
    print "<p>";
    print mysql_error();
    print "</p>";
    exit();
  }

  return true;
    
}

function get_json_url($url) {

  print "<pre>$url</pre>";

  $json = file_get_contents($url);
  return json_decode($json, true);

}

function get_one_tweet_from_twitter($tweetid) {

  $url = "http://twitter.com/statuses/show/$tweetid.json";

  return get_json_url($url);

}

function get_tweets_from_twitter($user, $hashtag, $last) {

  $url = "http://search.twitter.com/search.json?q=%23{$hashtag}+from%3A{$user}&since_id={$last}";

  return get_json_url($url);

}

function get_novel_data($novelname) {
  if (! verify_id($novelname) ) return array();

  $sql = 
    "select `twittername`, `humanname`, `hashtag`, `lastid` " .
    "from `tales` " .
    "where `novelname`='$novelname'";

  $result = mysql_query($sql);

  if (! $result ) {
    echo( "<p>error 4</p>" );
    exit();
  }
 
  $tale_info = array();
  if ($row = mysql_fetch_array($result) ) {
    $tale_info['twittername'] = $row['twittername'];
    $tale_info['humanname'] = $row['humanname'];
    $tale_info['hashtag'] = $row['hashtag'];
    $tale_info['lastid'] = $row['lastid'];
  }

  return $tale_info;

}

function add_tweet_to_novel($novelname, $tweet, $when) {
  if (! verify_id($novelname) ) return false;
  if (! verify_date($when) ) return false;
  $tweet = mysql_real_escape_string($tweet);

  $sql = 
    "insert into `tweets` ".
    "(`novelname`, `when`, `text`) ".
    "values ('$novelname', str_to_date('$when', '%Y%m%d%H%i%s'), '$tweet')";

  if (mysql_query($sql)) {
    print "<p>inserted $tweet at $when</p>";
  } else {
    print "<p>error 5</p>";
    print "<p>";
    print mysql_error();
    print "</p>";
    exit();
  }

  return true;
}

function pull_one_tweet_from_twitter($novelname, $twittername, $hashtag, $tweetid) {
  $json = get_one_tweet_from_twitter($tweetid);

  $tweet_data = array();

  if ($json !== false) {
    if ($json['user']['screen_name'] == $twittername) {
      $tweet_data['text']        = $json['text'];
      $when = strtotime($json['created_at']);
      $tweet_data['when']        = date('YmdHis', $when);
    } else {
      print "<p>error 12</p>";
      exit();
    }
  } else {
    print "<p>error 11</p>";
    exit();
  }

  return $tweet_data;
}

function load_new_tweets_from_twitter($novelname, $twittername, $hashtag, $lastid) {
  $json = get_tweets_from_twitter($twittername, $hashtag, $lastid);

  $max_id = $json['max_id'];

  $found_results = false;

  while ( count($json['results']) != 0 ) {

    $found_results = true;

    foreach ($json['results'] as $tweet) {
      $when = strtotime($tweet['created_at']);
      if ($when === false) {
	print "<p>error 6</p>";
	exit();
      } else {
	$when = date('YmdHis', $when);
      }
      if (! add_tweet_to_novel($novelname, html_entity_decode($tweet['text']), $when) ) {
	print "<p>error 7</p>";
	exit();
      }
    }

    usleep(600000);

    $json = get_json_url('http://search.twitter.com/search.json' . $json['next_page']);

  }

  update_novel_lastid($novelname, $max_id);

  return $found_results;

}

function add_mark_for_tweet($novelname, $when, $marktypeid) {

  if (! verify_id($novelname) ) return false;
  if (! verify_date($when) ) return false;
  if (! verify_number($marktypeid) ) return false;

  $sql = 
    "insert into `marks` " .
    "(`novelname`, `when`, `marktypeid`) " .
    "values ('$novelname', str_to_date('$when', '%Y%m%d%H%i%s'), $marktypeid)";

  if (mysql_query($sql) ) {
    return 1;
  } else {
    return 0;
  }
  
}

function del_mark_for_tweet($novelname, $when, $marktypeid) {

  if (! verify_id($novelname) ) return false;
  if (! verify_date($when) ) return false;
  if (! verify_number($marktypeid) ) return false;

  $sql = 
    "delete from `marks` " .
    "where `novelname` = '$novelname' " .
    "  and `when`      = str_to_date('$when', '%Y%m%d%H%i%s') " .
    "  and `marktypeid`= $marktypeid";

  if (mysql_query($sql) ) {
    return 1;
  } else {
    return 0;
  }
  
}

function add_page_for_tweet($novelname, $when, $page) {
  
  if (! verify_id($novelname) ) return false;
  if (! verify_date($when) ) return false;
  if (! verify_number($page) ) return false;

  $sql = 
    "insert into `pages` " .
    "(`novelname`, `page`, `when`) " .
    "values ('$novelname', $page, str_to_date('$when', '%Y%m%d%H%i%s'))";

  if (mysql_query($sql) ) {
    return 1;
  } else {
    return 0;
  }
  
}

function get_max_page_for_novel($novelname) {

  if (! verify_id($novelname) ) return false;

  $sql = 
    "select max(`page`) last_page " .
    "from `pages` " .
    "where `novelname` = '$novelname'";

  $result = mysql_query($sql);

  if (! $result) {
    return 0;
  }
  
  if ($row = mysql_fetch_array($result) ) {
    return $row['last_page'];
  } else {
    return 0;
  }

}

?>