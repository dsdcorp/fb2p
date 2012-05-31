<?php
if (!defined("INC_PROGRESS_H")){
   define("INC_PROGRESS_H", TRUE);

$GLOBALS['pstart']=false;
$GLOBALS['plen']=0;
$GLOBALS['ppref']='';

$GLOBALS['progress_data']=false;

function progress_set_divider($div) {
   $pdata=&$GLOBALS['progress_data'];
   $pdata['prc_div']=$div;
}

function progress_init() {
    if (!$GLOBALS['progress_data']) {
      $pdata=array();
      $pdata['prc_div']='%d';
      $pdata['si_min']=60;
      $pdata['si_hour']=$pdata['si_min']*60;
      $pdata['si_day']=$pdata['si_hour']*24;
      $pdata['si_mon']=$pdata['si_day']*30;
      $pdata['si_year']=$pdata['si_mon']*12;
      $pdata['pstack']=array();
      $pdata['pstart']=microtime(true);
      $pdata['plen']=0;
      $GLOBALS['progress_data']=$pdata;
    }

    $current=array();
    $current['pstart']=microtime(true);
    $current['plen']=0;
    $current['ppref']='';
    $current['val']=0;
    $current['max']=0;
    $GLOBALS['progress_data']['pstack'][]=$current;
    $ind=count($GLOBALS['progress_data']['pstack'])-1;
    $GLOBALS['progress_data']['current']=$ind;

    return ($ind>0)?false:true;
}

function progress_fmt_time($timeval, $in_float=false) {
   $pdata=&$GLOBALS['progress_data'];
//   $timeval=(int)$timeval;
   if ($timeval<=0) return '0:00:00';
   $r_year=(int)($timeval/$pdata['si_year']); $timeval=$timeval-($r_year*$pdata['si_year']);
   $r_mon=(int)($timeval/$pdata['si_mon']); $timeval=$timeval-($r_mon*$pdata['si_mon']);
   $r_day=(int)($timeval/$pdata['si_day']); $timeval=$timeval-($r_day*$pdata['si_day']);
   $r_hour=(int)($timeval/$pdata['si_hour']); $timeval=$timeval-($r_hour*$pdata['si_hour']);
   $r_min=(int)($timeval/$pdata['si_min']); $timeval=$timeval-($r_min*$pdata['si_min']);
   $r_sec=$timeval;
   $tf_str='%02d:%02d:%02d';
   if ($in_float) $tf_str='%02d:%02d:%06.6f';
   $result=sprintf($tf_str, $r_hour, $r_min, $r_sec);
   if ($r_day>0) $result=$r_day.'d '.$result;
   if ($r_mon>0) $result=$r_mon.'m '.$result;
   if ($r_year>0) $result=$r_year.'y '.$result;
   return $result;
}

function progress_print($timeval, $is_first=false) {
    $gpd=&$GLOBALS['progress_data'];
    $pd=&$GLOBALS['progress_data']['pstack'][$GLOBALS['progress_data']['current']];
    if (!$is_first) print(str_repeat("\x08", $gpd['plen']));
    $prc=(progress_calc_global_prc()*100);
    $prc=sprintf($gpd['prc_div'], $prc);
    $str=$pd['ppref'].
         "[".str_pad($pd['val'], 8, ' ', STR_PAD_LEFT)."]".
         "[".str_pad($prc, 8, ' ', STR_PAD_LEFT)."%]".
         "[".progress_fmt_time($timeval)."]";
    $str=str_pad($str, $gpd['plen']);
    $gpd['plen']=strlen($str);
    print($str);
    unset($str);
    unset($prc);
}

function progress_calc_global_prc() {
   $gpd=&$GLOBALS['progress_data'];
   $len=count($gpd['pstack']);
   $prc=0;
   for ($i=$len-1;$i>=0;$i--) {
       $pd=$gpd['pstack'][$i];
       $val=$pd['val']+$prc;
       $prc=($pd['max']<=0)?0:$val/$pd['max'];
//       fprintf(STDERR,$pd['val'].':'.$pd['max']."\n");
   }
//   fprintf(STDERR,"prc: $prc\n");
   return $prc;
}

function progress_calc_rtime() {
    $gpd=&$GLOBALS['progress_data'];
    $prc=progress_calc_global_prc();
    if ($prc==0) return 0;
    $tm_el = microtime(true)-$gpd['pstart'];
    $tm_max = $tm_el/$prc;
    $tm_rem=$tm_max-$tm_el;
    return $tm_rem;
}

function progress_start($max, $prefix='') {
    $is_first=progress_init();
    $pd=&$GLOBALS['progress_data']['pstack'][$GLOBALS['progress_data']['current']];
    $pd['plen']=0;
    $pd['ppref']=$prefix;
    $pd['val']=0;
    $pd['max']=$max;
    $rtime=progress_calc_rtime();
    progress_print($rtime, $is_first);
}

function progress_do($val) {
    $pd=&$GLOBALS['progress_data']['pstack'][$GLOBALS['progress_data']['current']];
    $pd['val']=$val;
    $rtime=progress_calc_rtime();
    progress_print($rtime);
}

function progress_end($report=false) {
    $gpd=&$GLOBALS['progress_data'];
    $pd=&$GLOBALS['progress_data']['pstack'][$GLOBALS['progress_data']['current']];

    $tm_el = microtime(true)-$gpd['pstart'];
    $str="[Elapsed time: ".progress_fmt_time($tm_el, true)."]";

    $ind=count($gpd['pstack'])-1;
    if ($ind>=0) array_pop($gpd['pstack']);
    $ind--;
    if ($ind>=0) {
       $GLOBALS['progress_data']['current']=$ind;
    } else {
       $s=str_repeat("\x08", $gpd['plen']).str_repeat("\x20", $gpd['plen']).str_repeat("\x08", $gpd['plen']);
       print($s);
       unset($s);
       if ($report) print("$str\n");
       $GLOBALS['progress_data']=false;
    }
    return $str;                 
}

} // end incl_h
?>
