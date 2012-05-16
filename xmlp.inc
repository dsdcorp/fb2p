<?php                                                
if (!defined("INC_XMLP_H")){
   define("INC_XMLP_H", TRUE);

define("XMLP_TT_UNKNOWN", 0x00); // вид тега неопределен
define("XMLP_TT_OPEN", 0x01); // открывающий вида <*>
define("XMLP_TT_CLOSE", 0x02); // зарывающий вида </*>
define("XMLP_TT_SINGLE", 0x03); // одиночный вида <*/>
define("XMLP_TT_XML", 0x04); // тупо тег вида < ?*? > (только без пробелов)
define("XMLP_TT_SPECIAL", 0x05); // специальный тег типа <DOCTYPE>

define("XMLP_UT_LEAVE", 0x00); // utags(неизвестные и недопустимые теги) останутся в результате как есть.
define("XMLP_UT_CONVERT", 0x01); // utags будут преобразованы в html entities.
define("XMLP_UT_CUT", 0x02); // utags будут вырезаны, но их внутренний контент останется. только <*> будут вырезаны.
define("XMLP_UT_CUT_FULL", 0x03); // utags будут вырезаны вместе с контентом. Все между открывающимися и закрывающимися тегами будет вырезано: <*>*</*>, так же влияет на незакрытые теги - отрежет все до ближайшего валидного тега.
define("XMLP_UT_CUT_SMART", 0x02); // utags will disappear with their content, but not affects on unclosed tags. Currently not implemented, alias for XMLP_UT_CUT.

define("XMLP_TPS_FREE", 0x00); // свободный парсинг всего что похоже на теги
define("XMLP_TPS_STRICT", 0x01); // контролируется внутренний формат тега.
define("XMLP_TPS_SAFE", 0x02); // XMLP_TPS_STRICT + ограничение длины частей тега.

define("XMLP_SDM_SIMPLE", 0x00);
define("XMLP_SDM_OFFSET", 0x01);
define("XMLP_SDM_PLAIN", 0x02);
define("XMLP_SDM_DOUBLE", 0x03);
define("XMLP_SDM_MERGED", 0x04);
define("XMLP_SDM_MERGED_NOEMPTY", 0x05);

$GLOBALS['XMLP_mem_usage_report']=false;

$GLOBALS['XMLP_curr_enc']='UTF-8';

$GLOBALS['XMLP_FB2_elements']=array (
  '--' => XMLP_TT_OPEN,
  'xml' => XMLP_TT_XML,
  'fictionbook' => XMLP_TT_OPEN,
  'a' => XMLP_TT_OPEN,
  'annotation' => XMLP_TT_OPEN,
  'author' => XMLP_TT_OPEN,
  'binary' => XMLP_TT_OPEN,
  'body' => XMLP_TT_OPEN,
  'book-name' => XMLP_TT_OPEN,
  'book-title' => XMLP_TT_OPEN,
  'cite' => XMLP_TT_OPEN,
  'city' => XMLP_TT_OPEN,
  'code' => XMLP_TT_OPEN,
  'coverpage' => XMLP_TT_OPEN,
  'custom-info' => XMLP_TT_OPEN,
  'date' => XMLP_TT_OPEN,
  'description' => XMLP_TT_OPEN,
  'document-info' => XMLP_TT_OPEN,
  'email' => XMLP_TT_OPEN,
  'emphasis' => XMLP_TT_OPEN,
  'empty-line' => XMLP_TT_SINGLE,
  'epigraph' => XMLP_TT_OPEN,
  'first-name' => XMLP_TT_OPEN,
  'genre' => XMLP_TT_OPEN,
  'history' => XMLP_TT_OPEN,
  'home-page' => XMLP_TT_OPEN,
  'id' => XMLP_TT_OPEN,
  'image' => XMLP_TT_SINGLE,
  'isbn' => XMLP_TT_OPEN,
  'keywords' => XMLP_TT_OPEN,
  'lang' => XMLP_TT_OPEN,
  'last-name' => XMLP_TT_OPEN,
  'middle-name' => XMLP_TT_OPEN,
  'nickname' => XMLP_TT_OPEN,
//  'output' => XMLP_TT_OPEN,
//  'output-document-class' => XMLP_TT_OPEN,
  'p' => XMLP_TT_OPEN,
//  'part' => XMLP_TT_OPEN,
  'poem' => XMLP_TT_OPEN,
  'program-used' => XMLP_TT_OPEN,
  'publish-info' => XMLP_TT_OPEN,
  'publisher' => XMLP_TT_OPEN,
  'section' => XMLP_TT_OPEN,
  'sequence' => XMLP_TT_SINGLE,
  'src-lang' => XMLP_TT_OPEN,
  'src-ocr' => XMLP_TT_OPEN,
//  'src-title-info' => XMLP_TT_OPEN,
  'src-url' => XMLP_TT_OPEN,
  'stanza' => XMLP_TT_OPEN,
  'strikethrough' => XMLP_TT_OPEN,
  'strong' => XMLP_TT_OPEN,
  'style' => XMLP_TT_OPEN,
  'stylesheet' => XMLP_TT_OPEN,
  'sub' => XMLP_TT_OPEN,
  'subtitle' => XMLP_TT_OPEN,
  'sup' => XMLP_TT_OPEN,
  'table' => XMLP_TT_OPEN,
  'td' => XMLP_TT_OPEN,
  'text-author' => XMLP_TT_OPEN,
  'th' => XMLP_TT_OPEN,
  'title' => XMLP_TT_OPEN,
  'title-info' => XMLP_TT_OPEN,
  'tr' => XMLP_TT_OPEN,
  'translator' => XMLP_TT_OPEN,
  'v' => XMLP_TT_OPEN,
  'version' => XMLP_TT_OPEN,
  'year' => XMLP_TT_OPEN,
);

$GLOBALS['XMLP_HTML_elements']=array (
  '--' => XMLP_TT_OPEN,
  'a' => XMLP_TT_OPEN,
  'abbr' => XMLP_TT_OPEN,
  'acronym' => XMLP_TT_OPEN,
  'address' => XMLP_TT_OPEN,
  'applet' => XMLP_TT_OPEN,
  'area' => XMLP_TT_SINGLE,
  'b' => XMLP_TT_OPEN,
  'base' => XMLP_TT_SINGLE,
  'basefont' => XMLP_TT_SINGLE,
  'bdo' => XMLP_TT_OPEN,
  'big' => XMLP_TT_OPEN,
  'blockquote' => XMLP_TT_OPEN,
  'body' => XMLP_TT_OPEN,
  'br' => XMLP_TT_SINGLE,
  'button' => XMLP_TT_OPEN,
  'caption' => XMLP_TT_OPEN,
  'center' => XMLP_TT_OPEN,
  'cite' => XMLP_TT_OPEN,
  'code' => XMLP_TT_OPEN,
  'col' => XMLP_TT_SINGLE,
  'colgroup' => XMLP_TT_OPEN,
  'dd' => XMLP_TT_OPEN,
  'del' => XMLP_TT_OPEN,
  'dfn' => XMLP_TT_OPEN,
  'dir' => XMLP_TT_OPEN,
  'div' => XMLP_TT_OPEN,
  'dl' => XMLP_TT_OPEN,
  'doctype' => XMLP_TT_SPECIAL,
  'dt' => XMLP_TT_OPEN,
  'em' => XMLP_TT_OPEN,
  'fieldset' => XMLP_TT_OPEN,
  'font' => XMLP_TT_OPEN,
  'form' => XMLP_TT_OPEN,
  'frame' => XMLP_TT_SINGLE,
  'frameset' => XMLP_TT_OPEN,
  'h1' => XMLP_TT_OPEN,
  'h6' => XMLP_TT_OPEN,
  'head' => XMLP_TT_OPEN,
  'hr' => XMLP_TT_SINGLE,
  'html' => XMLP_TT_OPEN,
  'i' => XMLP_TT_OPEN,
  'iframe' => XMLP_TT_OPEN,
  'img' => XMLP_TT_SINGLE,
  'input' => XMLP_TT_OPEN,
  'ins' => XMLP_TT_OPEN,
  'kbd' => XMLP_TT_OPEN,
  'label' => XMLP_TT_OPEN,
  'legend' => XMLP_TT_OPEN,
  'li' => XMLP_TT_OPEN,
  'link' => XMLP_TT_SINGLE,
  'map' => XMLP_TT_OPEN,
  'menu' => XMLP_TT_OPEN,
  'meta' => XMLP_TT_SINGLE,
  'noframes' => XMLP_TT_OPEN,
  'noscript' => XMLP_TT_OPEN,
  'object' => XMLP_TT_OPEN,
  'ol' => XMLP_TT_OPEN,
  'optgroup' => XMLP_TT_OPEN,
  'option' => XMLP_TT_OPEN,
  'p' => XMLP_TT_OPEN,
  'param' => XMLP_TT_SINGLE,
  'pre' => XMLP_TT_OPEN,
  'q' => XMLP_TT_OPEN,
  's' => XMLP_TT_OPEN,
  'samp' => XMLP_TT_OPEN,
  'script' => XMLP_TT_OPEN,
  'select' => XMLP_TT_OPEN,
  'small' => XMLP_TT_OPEN,
  'span' => XMLP_TT_OPEN,
  'strike' => XMLP_TT_OPEN,
  'strong' => XMLP_TT_OPEN,
  'style' => XMLP_TT_OPEN,
  'sub' => XMLP_TT_OPEN,
  'sup' => XMLP_TT_OPEN,
  'table' => XMLP_TT_OPEN,
  'tbody' => XMLP_TT_OPEN,
  'td' => XMLP_TT_OPEN,
  'textarea' => XMLP_TT_OPEN,
  'tfoot' => XMLP_TT_OPEN,
  'th' => XMLP_TT_OPEN,
  'thead' => XMLP_TT_OPEN,
  'title' => XMLP_TT_OPEN,
  'tr' => XMLP_TT_OPEN,
  'tt' => XMLP_TT_OPEN,
  'u' => XMLP_TT_OPEN,
  'ul' => XMLP_TT_OPEN,
  'var' => XMLP_TT_OPEN,
  'xmp' => XMLP_TT_OPEN,
);

$GLOBALS['XMLP_entities1']=array (
    '&' => '&amp;',
    '"' => '&quot;',
    '<' => '&lt;',
    '>' => '&gt;',
    '§' => '&sect;',
    '©' => '&copy;',
    '«' => '&laquo;',
    '®' => '&reg;',
    '°' => '&deg;',
    '±' => '&plusmn;',
    'µ' => '&micro;',
    '¶' => '&para;',
    '№' => '&sup1;',
    '»' => '&raquo;',
);

$GLOBALS['XMLP_entities']=array (
    '&' => '&amp;',
    '<' => '&lt;',
    '>' => '&gt;',
);


function xmlp_ent_hash2regex_conv($eh) {
   $res=array(array(),array(),array(),array());
   $keys=array_keys($eh);
   $l=count($keys);
   for($i=0;$i<$l;$i++) {
       $key=$keys[$i];
       $val=$eh[$key];
       $res[0][$i]='/'.$key.'/i';
       $res[1][$i]=$val;
       $res[2][$i]='/'.$val.'/i';
       $res[3][$i]=$key;
   }
   return $res;
}

$GLOBALS['XMLP_entities_a']=xmlp_ent_hash2regex_conv($GLOBALS['XMLP_entities']);

function xmlp_unquot_name($name) {
  $s=trim(html_entity_decode($name, ENT_QUOTES, 'cp1251'));
  if (preg_match('/^([\\\'\"]{1})(.*)(\1)$/is', $s, $matches)) $s=trim($matches[2]);
  return $s;
}

function xmlp_un_utf($str) {
  return xmlp_unquot_name(@iconv( 'UTF-8', 'windows-1251', $str));
}

function xmlp_to_1251($str, $src_enc='UTF-8') {
//  fprintf(STDERR, "xmlp_to_1251: [".$str."]\n");
  $dest_enc='windows-1251';
  $src_enc=strtolower($src_enc);
  $s=($src_enc==$dest_enc)?$str:@iconv($src_enc, $dest_enc, $str);
  return xmlp_unquot_name($s);
}

function xmlp_get_tag_pattern($config=false) {
  $config=xmlp_config($config, array(
     'name_first_alpha'=>false,
     'name_len'=>20,
     'content_len'=>512,
     'pattern_type'=>XMLP_TPS_STRICT,
     'include_comments'=>true,
  ));

  $tag_name_pat='[^\s\>\<]*';
  $tag_content_pat='[\s][^\>\<]*?';
  if ($config['pattern_type']==XMLP_TPS_STRICT) {
    $tag_name_pat='[a-zA-Z\.\:\-\_\d]*?';
    $tag_content_pat='[\s][^\>\<]*?';
  } else if ($config['pattern_type']==XMLP_TPS_SAFE) {
    $tag_name_pat='[a-zA-Z\.\:\-\_\d]{1,'.$config['name_len'].'}';
    $tag_content_pat='[\s][^\>\<]{0,'.$config['content_len'].'}';
  }
  if ($config['name_first_alpha']) $tag_name_pat='[a-zA-Z]'.$tag_name_pat;
  $tag_pat='\<([\?\/\!]?)('.$tag_name_pat.')('.$tag_content_pat.')?([\?\/]?)(\>)';
  if ($config['include_comments']) {
    $tag_comment_pat1='\<\!\-\-';
    $tag_comment_pat2='\-\-\>';
    return $tag_comment_pat1.'|'.$tag_comment_pat2.'|'.$tag_pat;
  }
  return $tag_pat;
}

function xmlp_get_tag_attr_pattern() {
   return '([^\s]+)\=(?:([\'\"]{1})(.*?)(\2)|([^\s]+))';
//   return '([^\s]+)\=(?:([\\\'\"]{1})(.*?)(\2)|([^\s]+))';
}

function xmlp_get_tag_attributes($tag_text) {
  $attr=array();
  $offset=0;                                   
  $r=preg_match_all('/'.xmlp_get_tag_attr_pattern().'/is', $tag_text, $matches, PREG_OFFSET_CAPTURE | PREG_SET_ORDER, 0);
  if ($r==false || $r<=0) return false;
  $mlen=count($matches);
  for ($i=0;$i<$mlen;$i++) {
     $name=(string)$matches[$i][1][0];
     $val=(string)$matches[$i][3][0];
     if (count($matches[$i])==6) $val=(string)$matches[$i][5][0];
     $attr[$name]=$val;
  }
  if (count($attr)==0) return false;
  return $attr;
}

function xmlp_parse_tag_match_old($match, $key=-1, $tags_hash=false) {
   $tag=array();

   if ($key>=0) $tag['ind']=$key;
   if (count($match)==1) {
      $tag['offset']=$match[0][1];
      $tag['length']=strlen($match[0][0]);
      $tag['end_offset']=$tag['offset']+$tag['length'];
      $tag['name']='--';
      $tag['lname']=$tag['name'];
      $tag['known']=(is_array($tags_hash))?xmlp_get_prop($tags_hash, $tag['lname'], false):true;
      $tag['type']=($match[0][0]=='<!--')?XMLP_TT_OPEN:XMLP_TT_CLOSE;
      return $tag;
   }

   $tag['offset']=$match[0][1];
   $tag['end_offset']=$match[5][1]+1;
   $tag['length']=$tag['end_offset']-$tag['offset'];

   $tag['name']=$match[2][0];
   $tag['lname']=strtolower($tag['name']);

   $tag['known']=(is_array($tags_hash))?xmlp_get_prop($tags_hash, $tag['lname'], false):true;

   if ($match[1][0]=='!') {
      $tag['type']=XMLP_TT_SPECIAL;
   } else if ($match[4][0]=='/') {
      $tag['type']=XMLP_TT_SINGLE;
   } else if ($match[1][0]=='/') {
     $tag['type']=XMLP_TT_CLOSE;
   } else if ($match[1][0]=='?' && $match[4][0]=='?') {
     $tag['type']=XMLP_TT_XML;
   } else $tag['type']=XMLP_TT_OPEN;

   if ($match[3][1]>0) {
     if ($tag['type']==XMLP_TT_SPECIAL) {
        $tag['attributes']=$match[3][0];
     } else {
       $tag['attributes']=xmlp_get_tag_attributes($match[3][0]);
     }
     if (!$tag['attributes']) unset($tag['attributes']);
   }

   return $tag;
}

function xmlp_parse_tag_match($match, $key=-1, $tags_hash=false) {
   $tag=array();
   if ($key>=0) $tag['ind']=$key;
   if (count($match)==1) {
      $tag['name']='--';
      $tag['lname']=$tag['name'];
      $tag['known']=(is_array($tags_hash))?xmlp_get_prop($tags_hash, $tag['lname'], false):true;
      $tag['type']=($match[0]=='<!--')?XMLP_TT_OPEN:XMLP_TT_CLOSE;
      return $tag;
   }

   $tag['name']=$match[2];
   $tag['lname']=strtolower($tag['name']);

   $tag['known']=(is_array($tags_hash))?xmlp_get_prop($tags_hash, $tag['lname'], false):true;

   if ($match[1]=='!') {
      $tag['type']=XMLP_TT_SPECIAL;
   } else if ($match[4]=='/') {
      $tag['type']=XMLP_TT_SINGLE;
   } else if ($match[1]=='/') {
     $tag['type']=XMLP_TT_CLOSE;
   } else if ($match[1]=='?' && $match[4]=='?') {
     $tag['type']=XMLP_TT_XML;
   } else $tag['type']=XMLP_TT_OPEN;

   if (isset($match[3]{1})) {
     if ($tag['type']==XMLP_TT_SPECIAL) {
        $tag['attributes']=$match[3];
     } else {
        $tag['attributes']=xmlp_get_tag_attributes($match[3]);
     }
     if (!$tag['attributes']) unset($tag['attributes']);
   }

   return $tag;
}

function xmlp_get_tags($data, $offset=0) {
   $r=preg_match_all('/'.xmlp_get_tag_pattern().'/is', $data, $matches, PREG_OFFSET_CAPTURE | PREG_SET_ORDER, $offset);
   if ($r && $r>0) array_walk($matches, create_function('&$item, $key','$item = xmlp_parse_tag_match_old($item, $key);'));
   return $matches;
}

function xmlp_get_tag($data, $offset=0) {
   if (!preg_match('/'.xmlp_get_tag_pattern().'/is', $data, $matches, PREG_OFFSET_CAPTURE, $offset)) return false;
   return xmlp_parse_tag_match_old($matches);
}

function xmlp_ent_dec_str($s) {
  return preg_replace($GLOBALS['XMLP_entities_a'][2], $GLOBALS['XMLP_entities_a'][3], $s);
//  return html_entity_decode($s, ENT_QUOTES, 'cp1251');
}

function xmlp_ent_enc_str($s) {
  return preg_replace($GLOBALS['XMLP_entities_a'][0], $GLOBALS['XMLP_entities_a'][1], $s);
//  return htmlspecialchars($s, ENT_QUOTES, 'ISO-8859-1', false);
}                                     

function xmlp_split_data($pattern, $data, $method=XMLP_SDM_DOUBLE) {
  if ($method==XMLP_SDM_SIMPLE) {
    return explode($pattern, $data);
  } else if ($method==XMLP_SDM_OFFSET) {
    $r=preg_match_all($pattern, $data, $tags, PREG_OFFSET_CAPTURE | PREG_SET_ORDER, 0);
    if ($r==false || $r<=0) return false;
    return $tags;
  } else if ($method==XMLP_SDM_PLAIN) {
    $r=preg_match_all($pattern, $data, $tags, PREG_SET_ORDER, 0);
    if ($r==false || $r<=0) return false;
    return $tags;
  }
  $cdata=preg_split($pattern, $data, -1, 0);
  $r=preg_match_all($pattern, $data, $tags, PREG_SET_ORDER, 0);
  if ($r==false || $r<=0) return false;
  $cl=count($cdata);
  $tl=count($tags);
  if ($cl!=$tl+1) return false;
  if ($method==XMLP_SDM_DOUBLE) return array($tags, $cdata);
  $ret=array();
  $i=0;
  for ($i=0;$i<$tl;$i++) {
    if ($methos!=XMLP_SDM_MERGED_NOEMPTY || isset($cdata[$i]{0})) $ret[]=$cdata[$i];
    $ret[]=$tags[$i];
  }
  if ($i>0 && isset($cdata[$i])) $ret[]=$cdata[$i];
  return ($ret);
}

function xmlp_data2struct($data, $config=false) {
  $config=xmlp_config($config, array(
     'struct_only'=>false,
     'unknown_tags_processing'=>XMLP_UT_LEAVE,
     'tags_processing'=>false,
     'strip_comments'=>false,
     'tags_hash'=>false,
  ));
  if ($config['unknown_tags_processing']==XMLP_UT_LEAVE) $config['tags_hash']=false;
  
xmlp_memreport('xmlp_data2struct 1');
  if (!$config['struct_only']) {
    $cdata=preg_split('/'.xmlp_get_tag_pattern($config['tags_processing']).'/is', $data, -1, 0);
//    array_walk($cdata, create_function('&$item, $key','$item = xmlp_ent_dec_str($item);'));
//    $cdata=explode("\x00", xmlp_ent_dec_str(implode("\x00", $cdata)));
//    return $cdata;
  }

  $r=preg_match_all('/'.xmlp_get_tag_pattern($config['tags_processing']).'/is', $data, $tags, PREG_SET_ORDER, 0);
  if ($r==false || $r<=0) return false;
//  return $tags;
xmlp_memreport('xmlp_data2struct 2');
  if (!$config['struct_only'] && count($cdata)!=count($tags)+1) return false;
//  if ($config['struct_only']) { return $tags; } else return array($tags, $cdata);


  // main cycle
  $pr_ind=-1; // previous tag index
  $p_ind=-1; // previous unclosed tag index
  $res=array();
  $arr=&$res;
  $tlen=count($tags);
  $parent_stack=array();
  $psti=-1; // parent stack items max_ind ( $psti+1 = stack path length )
  $ut_plain='';
  $sct_done=false;
  $tpsti=-1;
  $tp_ind=-1;
  $r_ind=0;
  $comment_processing=false;

  $utags=array();
  $recovered_tags=array();
  $errors=array();
  $i=0;
  for ($i=0;$i<$tlen;$i++) {
    $ut_plain=$tags[$i][0];
    $tags[$i]=xmlp_parse_tag_match($tags[$i], $i, $config['tags_hash']); //continue;
    if ($comment_processing) {
      if ($tags[$i]['name']=='--') {
         if ($tags[$i]['type']!=XMLP_TT_CLOSE) continue;
      } else {
        if (!$config['struct_only']) {
           if ($i>0 || isset($cdata[$i]{0})) $arr[]=$cdata[$i];
           $arr[]=$ut_plain;
        }
        continue;
      }
      $comment_processing=false;
    }
    if (!$tags[$i]['known'] && $config['unknown_tags_processing']==XMLP_UT_CUT_FULL) continue;
    if (!$config['struct_only']) {
       if ($i>0 || isset($cdata[$i]{0})) {
         $arr[]=xmlp_ent_dec_str($cdata[$i]);
//         $arr[]=$cdata[$i];
       }   
       if (!$tags[$i]['known'] && $config['unknown_tags_processing']==XMLP_UT_CONVERT) {
           $arr[]=xmlp_ent_dec_str($ut_plain);
           continue;
       }
    }           
    if ($config['unknown_tags_processing']==XMLP_UT_LEAVE || $tags[$i]['known']) switch ($tags[$i]['type']) {
      case XMLP_TT_SPECIAL:
      case XMLP_TT_XML:
      case XMLP_TT_SINGLE:
        $arr[]=&$tags[$i];
        break;
      case XMLP_TT_OPEN:
        $arr[]=&$tags[$i];
        $tags[$i]['content']=array();
        $arr=&$tags[$i]['content'];
        $psti++;
        $parent_stack[$psti]=$p_ind;
        $p_ind=$i;
        $comment_processing=($tags[$i]['name']=='--');
        break;
      case XMLP_TT_CLOSE:
        if ($p_ind<0 || $tags[$p_ind]['lname']!=$tags[$i]['lname']) {
            $utags[$tags[$p_ind]['ind']]=$tags[$i]['ind'];
            $sct_done=false;
            $r_ind=$p_ind;
            $tpsti=$psti;
            while (!$sct_done && $tpsti>-1) { // trying to search the actual opened tag with the same name
              $tp_ind=$parent_stack[$tpsti--];
              if ($tp_ind>-1 && $tags[$tp_ind]['lname']==$tags[$i]['lname']) {
                 // found. reverting the stack to actual closing position
                 $p_ind=$tp_ind;
                 $arr=&$tags[$tp_ind]['content'];
                 while ($psti>$tpsti) unset($parent_stack[$psti--]);
                 $sct_done=true;
              }
            }
            if ($sct_done) { $recovered_tags[]=$tags[$r_ind]['ind']; } else break;
        }
        if ($psti>=0) { // tag closing by shifting the stack.
           $p_ind=$parent_stack[$psti];
           unset($parent_stack[$psti--]);
           if ($p_ind>=0) { $arr=&$tags[$p_ind]['content']; } else $arr=&$res;
        } else {
           $p_ind=-1;
           $arr=&$res;
        }
        if ($config['strip_comments'] && $tags[$i]['name']=='--') array_pop($arr);
        break;
    }
    $pr_ind=$i; if ($p_ind==$pr_ind) $pr_ind=-1;
  }
  if (!$config['struct_only'] && $i>0 && isset($cdata[$i]{0})) $res[]=xmlp_ent_dec_str($cdata[$i]);
//  return $tags;
  $psti++;
  if (count($parent_stack)>0) { array_shift($parent_stack); $parent_stack[]=$p_ind; }

  if (count($utags)>0) $errors[]=array(
     'description'=>'Unknown closing tags: '.count($utags),
     'data'=>$utags,
  );

  if (count($recovered_tags)>0) $errors[]=array(
     'description'=>'Unclosed tags recovered: '.count($recovered_tags),
     'data'=>$recovered_tags,
  );

  if ($psti!=0) $errors[]=array(
     'description'=>'Problems leaved: '.$psti,
     'data'=>$parent_stack,
  );
                  
xmlp_memreport('xmlp_data2struct 3');
  if (count($errors)==0) $errors=false;
  return array($res, $errors);
}

function xmlp_data2struct_old($data, $config=false) {
  $config=xmlp_config($config, array(
     'struct_only'=>false,
     'unknown_tags_processing'=>XMLP_UT_LEAVE,
     'tags_processing'=>false,
     'strip_comments'=>false,
     'tags_hash'=>false,
  ));
  if ($config['unknown_tags_processing']==XMLP_UT_LEAVE) $config['tags_hash']=false;
xmlp_memreport('xmlp_data2struct_old 1');
  $tags=array();
  $r=preg_match_all('/'.xmlp_get_tag_pattern($config['tags_processing']).'/is', $data, $tags, PREG_OFFSET_CAPTURE | PREG_SET_ORDER, 0);
  if ($r==false || $r<=0) return false;
xmlp_memreport('xmlp_data2struct_old 2');
//  return $tags;

  // main cycle
  $pr_ind=-1; // previous tag index
  $p_ind=-1; // previous unclosed tag index
  $res=array();
  $arr=&$res;
  $tlen=count($tags);
  $parent_stack=array();
  $psti=-1; // parent stack items max_ind ( $psti+1 = stack path length )
  $ut_plain='';
  $sct_done=false;
  $tpsti=-1;
  $tp_ind=-1;
  $r_ind=0;
  $comment_processing=false;

  $utags=array();
  $recovered_tags=array();
  $errors=array();

  for ($i=0;$i<$tlen;$i++) {
    $ut_plain=$tags[$i][0][0];
    $tags[$i] = xmlp_parse_tag_match_old($tags[$i], $i, $config['tags_hash']); //continue;
    if ($comment_processing) {
      if ($tags[$i]['name']=='--') {
         if ($tags[$i]['type']!=XMLP_TT_CLOSE) continue;
      } else {
        if (!$config['struct_only']) {
           $gstart=$tags[$i-1]['end_offset'];
           $glen=$tags[$i]['offset']-$tags[$i-1]['end_offset'];
           if ($glen>0) $arr[]=substr($data, $gstart, $glen);
           $arr[]=$ut_plain;
        }
        continue;
      }
      $pr_ind=$i-1;
      $comment_processing=false;
    }
    if (!$tags[$i]['known'] && $config['unknown_tags_processing']==XMLP_UT_CUT_FULL) continue;
    if (!$config['struct_only']) {
      if ($pr_ind>=0) {
        $gstart=$tags[$pr_ind]['end_offset'];
        $glen=$tags[$i]['offset']-$tags[$pr_ind]['end_offset'];
        if ($glen>0) $arr[]=xmlp_ent_dec_str(substr($data, $gstart, $glen));
        if (!$tags[$i]['known'] && $config['unknown_tags_processing']==XMLP_UT_CONVERT) $arr[]=xmlp_ent_dec_str($ut_plain);
      } else if (count($arr)==0) {
        $gstart=($p_ind>=0)?$tags[$p_ind]['end_offset']:0;
        $glen=($p_ind>=0)?$tags[$i]['offset']-$tags[$p_ind]['end_offset']:$tags[$i]['offset'];
        if ($glen>0) $arr[]=xmlp_ent_dec_str(substr($data, $gstart, $glen));
        if (!$tags[$i]['known'] && $config['unknown_tags_processing']==XMLP_UT_CONVERT) $arr[]=xmlp_ent_dec_str($ut_plain);
      }
    }
    if ($config['unknown_tags_processing']==XMLP_UT_LEAVE || $tags[$i]['known']) switch ($tags[$i]['type']) {
      case XMLP_TT_SPECIAL:
      case XMLP_TT_XML:
      case XMLP_TT_SINGLE:
        $arr[]=&$tags[$i];
        break;
      case XMLP_TT_OPEN:
        $arr[]=&$tags[$i];
        $tags[$i]['content']=array();
        $arr=&$tags[$i]['content'];
        $psti++;
        $parent_stack[$psti]=$p_ind;
        $p_ind=$i;
        $comment_processing=($tags[$i]['name']=='--');
        break;
      case XMLP_TT_CLOSE:
        if ($p_ind<0 || $tags[$p_ind]['lname']!=$tags[$i]['lname']) {
            $utags[$tags[$p_ind]['ind']]=$tags[$i]['ind'];
            $sct_done=false;
            $r_ind=$p_ind;
            $tpsti=$psti;
            while (!$sct_done && $tpsti>-1) { // trying to search the actual opened tag with the same name
              $tp_ind=$parent_stack[$tpsti--];
              if ($tp_ind>-1 && $tags[$tp_ind]['lname']==$tags[$i]['lname']) {
                 // found. reverting the stack to actual closing position
                 $p_ind=$tp_ind;
                 $arr=&$tags[$tp_ind]['content'];
                 while ($psti>$tpsti) unset($parent_stack[$psti--]);
                 $sct_done=true;
              }
            }
            if ($sct_done) { $recovered_tags[]=$tags[$r_ind]['ind']; } else break;
        }
        if ($psti>=0) { // tag closing by shifting the stack.
           $p_ind=$parent_stack[$psti];
           unset($parent_stack[$psti--]);
           if ($p_ind>=0) { $arr=&$tags[$p_ind]['content']; } else $arr=&$res;
        } else {
           $p_ind=-1;
           $arr=&$res;
        }
        if ($config['strip_comments'] && $tags[$i]['name']=='--') array_pop($arr);
        break;
    }
    $pr_ind=$i; if ($p_ind==$pr_ind) $pr_ind=-1;
  }
  if (!$config['struct_only'] && $i>0) {
     $gstart=$tags[$tlen-1]['end_offset'];
     $glen=strlen($data)-$gstart;
     if ($glen>0) $res[]=xmlp_ent_dec_str(substr($data, $gstart, $glen));
  }
//  return $tags;
  $psti++;
  if (count($parent_stack)>0) { array_shift($parent_stack); $parent_stack[]=$p_ind; }

  if (count($utags)>0) $errors[]=array(
     'description'=>'Unknown closing tags: '.count($utags),
     'data'=>$utags,
  );

  if (count($recovered_tags)>0) $errors[]=array(
     'description'=>'Unclosed tags recovered: '.count($recovered_tags),
     'data'=>$recovered_tags,
  );

  if ($psti!=0) $errors[]=array(
     'description'=>'Problems leaved: '.$psti,
     'data'=>$parent_stack,
  );
                  
xmlp_memreport('xmlp_data2struct_old 3');
  if (count($errors)==0) $errors=false;
  return array($res, $errors);
}

function xmlp_get_tags_path(&$struct, $karr) {
  if (!is_array($struct)) return false;
  if (!is_array($karr)) return false;
  $kmax=count($karr)-1;
  if ($kmax<0) return false;

  $result=array();
  $stack=array();
  $sind=0;
  $stack[$sind]=array();
  $stack[$sind]['arr']=&$struct;
  $stack[$sind]['ind']=0;
  $stack[$sind]['max']=count($stack[$sind]['arr'])-1;
  $tag=false;

  while ($sind>-1) {
//     if ($sind<0) break; // обошли всю структуру. пора на выход.
     $tag=&$stack[$sind]['arr'][$stack[$sind]['ind']];                                           
/*
     $karr_v=array($karr[$sind], 0);
     if (preg_match('/^(.+)\<([\d]+)\>$/is', $karr[$sind], $matches)) {
        $karr_v[0]=$matches[1];
        $karr_v[1]=$matches[2];
     }
     return $karr_v;
*/
     if ($stack[$sind]['ind']>$stack[$sind]['max']) { // на текущем уровне итерация окончена - нет больше тегов. возвращаемся на предыдущий уровень.
        $sind--;
     } else if ($sind>$kmax) { // залезли дальше пути. врядли совпадет.
        $sind--;
     } else if (!is_array($tag)) { // это тег или скаляр(строка).
        $stack[$sind]['ind']++;
     } else if ($karr[$sind]!=$tag['lname']) { // имя не совпадает, тег не наш - пропускаем, итерируем дальше.
        $stack[$sind]['ind']++;
     } else {
       // наш тег на текущем уровне.
       $stack[$sind]['ind']++;
       if ($sind==$kmax) { // конечный тег(в пути). сохраняем в результат.
          $result[]=&$tag;
       } else if (array_key_exists('content',$tag) && is_array($tag['content'])) { // промежуточный тег(в пути). проваливаемся внутрь него.
          $sind++;
          $stack[$sind]=array();
          $stack[$sind]['arr']=&$tag['content'];
          $stack[$sind]['ind']=0;
          $stack[$sind]['max']=count($stack[$sind]['arr'])-1;
       }
     }
  }
  if (count($result)==0) return false;
  return $result;
}

function xmlp_compile_tag($tag, &$arr, $noenc=false) {
   $tag_ar=array();
   if ($tag['name']=='--') {
     $arr[]='<!--';
     if (array_key_exists('content',$tag)) xmlp_compile_struct($tag['content'], $arr, true);
     $arr[]='-->';
     return;
   }
   $tag_ar[]='<';
   if ($tag['type']==XMLP_TT_XML) $tag_ar[]='?';
   if ($tag['type']==XMLP_TT_SPECIAL) $tag_ar[]='!';
   $tag_ar[]=$tag['name'];
   if (array_key_exists('attributes',$tag)) {
     if ($tag['type']==XMLP_TT_SPECIAL) {
       $tag_ar[]=$tag['attributes'];
     } else {
       foreach($tag['attributes'] as $key=>$val) $tag_ar[]=' '.$key.'="'.xmlp_ent_enc_str($val).'"';
     }
   }
   if ($tag['type']==XMLP_TT_XML) $tag_ar[]='?';
   if ($tag['type']==XMLP_TT_SINGLE) $tag_ar[]='/';
   $tag_ar[]='>';
   $arr[]=implode('',$tag_ar);
   if ($tag['type']==XMLP_TT_OPEN) {
      if (array_key_exists('content',$tag)) xmlp_compile_struct($tag['content'], $arr, $noenc);
      $arr[]='</'.$tag['name'].'>';
   }                                                                           
}

function xmlp_compile_struct($struct, &$arr, $noenc=false) {
   if (!$struct) return false;
   if (!is_array($struct)) return false;
   $slen=count($struct);
   for ($i=0;$i<$slen;$i++) {
      if (!$struct[$i]) continue;
      if (is_array($struct[$i])) {
        xmlp_compile_tag($struct[$i], $arr, $noenc);
      } else if ($noenc) {
        $arr[]=$struct[$i];
      } else {
        $arr[]=xmlp_ent_enc_str($struct[$i]);
      }
   }
   return true;
}

function xmlp_struct2data($struct) {
  $out_arr=array();
  if (!xmlp_compile_struct($struct, $out_arr)) return false;
  return implode('',$out_arr);
}

function xmlp_get_inner_text($tag) {
   if (!array_key_exists('content', $tag)) return '';
   $slen=count($tag['content']);
   $result='';
   for ($i=0;$i<$slen;$i++) {
      if (is_array($tag['content'][$i])) {
        $result.=xmlp_get_inner_text($tag['content'][$i]);
      } else {
        $result.=(string)$tag['content'][$i];
      }
   }
   return $result;
}

function xmlp_find_1tag_val($struct, $karr, $default='') {
   $tags=xmlp_get_tags_path($struct, $karr);
//   fprintf(STDERR, "xmlp_find_1tag_val: [".$tags."]\n");
   if (!$tags) return $default;
   $res=xmlp_get_inner_text($tags[0]);
   return ($res=='')?$default:$res;
}

function xmlp_config($config=false, $defaults=false) {
  if (!$config || !is_array($config)) $config=array();
  if (!$defaults || !is_array($defaults)) $defaults=array();
  $keys=array_keys($defaults);
  $l=count($keys);
  for ($i=0;$i<$l;$i++) $config[$keys[$i]]=xmlp_get_prop($config, $keys[$i], $defaults[$keys[$i]]);
  return $config;
}

function xmlp_get_prop($tag, $prop_name, $default_=false, $find_only=false) {
   if (!array_key_exists($prop_name, $tag)) return $default_;
   return ($find_only)?true:$tag[$prop_name];
}

function xmlp_struct2book($struct) {
  $book=array();
//  $book['struct']=&$struct;
  $book['xml_header']=false;
  $book['xml_version']=1.0;
  $book['encoding']='UTF-8';
  $tags=xmlp_get_tags_path($struct, array('xml'));
  if ($tags) {
     $book['xml_header']=true;
     if ($attr=xmlp_get_prop($tags[0], 'attributes')) {
       $book['xml_version']=(float)xmlp_get_prop($attr, 'version', 0);
       $book['encoding']=(string)xmlp_get_prop($attr, 'encoding', 'UTF-8');
     }
  }

  $d_tags=xmlp_get_tags_path($struct, array('fictionbook','description')); if (!$d_tags) return false; $d_tags=xmlp_get_prop($d_tags[0],'content'); if (!$d_tags) return false;
  $ti_tags=xmlp_get_tags_path($d_tags, array('title-info')); if ($ti_tags) $ti_tags=xmlp_get_prop($ti_tags[0],'content');
  $di_tags=xmlp_get_tags_path($d_tags, array('document-info')); if ($di_tags) $di_tags=xmlp_get_prop($di_tags[0],'content');
  $pi_tags=xmlp_get_tags_path($d_tags, array('publish-info')); if ($pi_tags) $pi_tags=xmlp_get_prop($pi_tags[0],'content');
  $sti_tags=xmlp_get_tags_path($d_tags, array('src-title-info')); if ($sti_tags) $sti_tags=xmlp_get_prop($sti_tags[0],'content');

  $book['id']=xmlp_to_1251(xmlp_find_1tag_val($di_tags, array('id')), $book['encoding']);
  $book['isbn']=xmlp_to_1251(xmlp_find_1tag_val($pi_tags, array('isbn')), $book['encoding']);

  $book['title']=xmlp_to_1251(xmlp_find_1tag_val($ti_tags, array('book-title')), $book['encoding']);
  $book['lang']=xmlp_to_1251(xmlp_find_1tag_val($ti_tags, array('lang')), $book['encoding']);
  $book['src_lang']=xmlp_to_1251(xmlp_find_1tag_val($ti_tags, array('src-lang')), $book['encoding']);

  $book['genres']=array();
  xmlp_parse_genres($ti_tags, $book);
  xmlp_parse_genres($di_tags, $book);
  xmlp_parse_genres($pi_tags, $book);
  xmlp_parse_genres($sti_tags, $book);
   
  $book['authors']=array();
  xmlp_parse_autors($ti_tags, $book);
  xmlp_parse_autors($sti_tags, $book);

  $book['sequences']=array();
  xmlp_parse_sequences($ti_tags, $book);
  xmlp_parse_sequences($di_tags, $book);
  xmlp_parse_sequences($pi_tags, $book);
  xmlp_parse_sequences($sti_tags, $book);
  

  return $book;
}

function xmlp_parse_genres($i_tags, &$book) {
  if (!$i_tags) return false;
  $gtags=xmlp_get_tags_path($i_tags, array('genre'));
  if (!$gtags) return false;
  $len=count($gtags);
  for ($i=0;$i<$len;$i++) {
     $gstr=xmlp_to_1251(xmlp_get_inner_text($gtags[$i]), $book['encoding']);
     $gnames=explode(',', $gstr);
     foreach ($gnames as $gname) {
       $gname=trim($gname);
       if ($gname<>'') $book['genres'][]=$gname;
     }
  }
  return true;
}

function xmlp_parse_autors($i_tags, &$book) {
  if (!$i_tags) return false;
  $atags=xmlp_get_tags_path($i_tags, array('author'));
  if (!$atags) return false;
  $len=count($atags);
  $a=false;
  for ($i=0;$i<$len;$i++) {
     $a=xmlp_get_prop($atags[0],'content');
     $book['authors'][]=array(
                            xmlp_to_1251(xmlp_find_1tag_val($a, array('last-name')), $book['encoding']),
                            xmlp_to_1251(xmlp_find_1tag_val($a, array('first-name')), $book['encoding']),
                            xmlp_to_1251(xmlp_find_1tag_val($a, array('middle-name')), $book['encoding']),
                            xmlp_to_1251(xmlp_find_1tag_val($a, array('nickname')), $book['encoding']),
                            xmlp_to_1251(xmlp_find_1tag_val($a, array('home-page')), $book['encoding']),
                            xmlp_to_1251(xmlp_find_1tag_val($a, array('email')), $book['encoding']),
                            );
  }
  return true;
}

function xmlp_parse_sequences($i_tags, &$book) {
  if (!$i_tags) return false;
  $stags=xmlp_get_tags_path($i_tags, array('sequence'));
  if (!$stags) return false;
  $len=count($stags);
  $name='';
  $num=0;
  for ($i=0;$i<$len;$i++) {
     if ($attr=xmlp_get_prop($stags[$i], 'attributes')) {
       $name=(string)xmlp_to_1251(xmlp_get_prop($attr, 'name', ''), $book['encoding']);
       $num=(int)xmlp_to_1251(xmlp_get_prop($attr, 'number', ''), $book['encoding']);
       $book['sequences'][]=array($name, $num);
     }
  }
  return true;
}

function xmlp_memreport($pref) {
  if (!$GLOBALS['XMLP_mem_usage_report']) return;
  $mu=memory_get_usage(false);
  $mu1=memory_get_usage(true);
  fprintf(STDERR, $pref.": memory usage [".number_format($mu,0,'',' ').",".number_format($mu1,0,'',' ')."]\n");
}

} // end incl_h
?>
