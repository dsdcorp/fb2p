<?php

  ini_set("max_execution_time", "0");
  ini_set("memory_limit", "-1");

include("progress.inc");
include("xmlp.inc");


$GLOBALS['FB2_Elements']=array('xml', 'FictionBook','a','annotation','author','binary','body','book-name',
'book-title','cite','city','code','coverpage','custom-info','date','description','document-info','email',
'emphasis','empty-line','epigraph','first-name','genre','history','home-page','id','isbn','image','keywords',
'lang','last-name','middle-name','nickname','output-document-class','output','p','part','poem','program-used',
'publish-info','publisher','section','sequence','src-lang','src-ocr','src-title-info','src-url','stanza',
'strikethrough','strong','style','stylesheet','sub','subtitle','sup','table','td','text-author','th','title',
'title-info','tr','translator','v','version','year');

function fb2p_els2hash($els) {
  $hash=array();
  $elen=count($GLOBALS['FB2_Elements']);
  for ($i=0;$i<$elen;$i++) $hash[strtolower($GLOBALS['FB2_Elements'][$i])]=true;
  return $hash;
}

$GLOBALS['FB2_Hash']=fb2p_els2hash($GLOBALS['FB2_Elements']);


//$in_fname='0000008d.fb2';
$in_fname='example.fb2';
//$in_fname='276604.fb2';
//$in_fname='html.txt';
$fdata=@file_get_contents($in_fname);
$tp=xmlp_get_tag_pattern();
progress_start(1);
$tarr=xmlp_split_data('/'.$tp.'/is', $fdata, XMLP_SDM_PLAIN);
if (!$tarr) exit(0);
@file_put_contents('out_html.txt', var_export(array($tp, $tarr), true));
exit(0);
$tags=array();
$res=array();
$l=count($tarr);
for ($i=0;$i<$l;$i++) {
  $tags[$i]=xmlp_parse_tag_match($tarr[$i], $i, array());
  $tags[$i]['plain']=$tarr[$i][0];
//  $res[$tags[$i]['lname']]=get_ttn($tags[$i]['type']);
}
progress_end(true);
//ksort($res);
@file_put_contents('out_html.txt', var_export($tags, true));
//@file_put_contents('out_html.txt', implode("\n",$tags));
exit(0);
 




$max=100000;

progress_start(1);
for ($i=0;$i<$max;$i++) {
  array_key_exists('subtitle', $GLOBALS['FB2_Hash']);
}
progress_end(true);

progress_start(1);
for ($i=0;$i<$max;$i++) {
  array_search('subtitle', $GLOBALS['FB2_Elements']);
}
progress_end(true);
exit(0);


$test_str='abcdefghij';
$test_str=str_repeat(' ', 10000000);
//$test_str1=&$test_str;
$ind=200;
$cnt=10;
$max=1000;

$a1=array();
progress_start(1);
for ($i=0;$i<$max;$i++) {
  $a1[]=fb2p_substr($test_str, $ind, $cnt);
}
progress_end(true);

$a1=array();
$ts=(string)$test_str;
progress_start(1);
for ($i=0;$i<$max;$i++) {
  $a2[]=substr($ts, $ind, $cnt);
}
progress_end(true);


function fb2p_substr(&$data, $ind, $count) {
  $end=$ind+$count;
   
//  if (!isset($data{$end})) return ''; // strlen check replacement
//  if (!isset($data{$ind})) return ''; // strlen check replacement
                             
//  $s=str_repeat(' ', $count);
//  for ($i=$ind,$j=0;$i<$end;$i++,$j++) $s{$j}=$data{$i};
//  $s=''; for ($i=$ind;$i<$end && isset($data{$i});$i++) $s.=$data{$i};
  $s=''; for ($i=$ind;$i<$end;$i++) $s.=$data{$i};

  return $s;
}

function get_ttn($t) {
  $r="XMLP_TT_UNKNOWN";
  switch ($t) {
    case XMLP_TT_OPEN: $r="XMLP_TT_OPEN"; break;
    case XMLP_TT_CLOSE: $r="XMLP_TT_OPEN"; break;
    case XMLP_TT_SINGLE: $r="XMLP_TT_SINGLE"; break;
    case XMLP_TT_XML: $r="XMLP_TT_XML"; break;
    case XMLP_TT_SPECIAL: $r="XMLP_TT_SPECIAL"; break;
  }
  return $r;
}

?>
