<?php
function move_review($form, &$form_state) {

	/*Retrieve comemnt review data*/
	$cid = $form_state['values']['comment_id'];
	$comment_raw_data = db_query("SELECT comment.cid, comment.nid, comment.uid,
		comment.subject, comment.created,
		field_data_comment_body.comment_body_value AS 'body',
  field_data_field_comment_rating.field_comment_rating_rating AS 'rating' FROM comment
INNER JOIN field_data_field_comment_rating
ON comment.cid = field_data_field_comment_rating.entity_id
INNER JOIN field_data_comment_body
ON field_data_comment_body.entity_id = comment.cid
 WHERE comment.cid = :cid", array(':cid' => $cid,));
	foreach ($comment_raw_data as $comment_data) {
	$nid = $comment_data->nid;
	$cid = $comment_data->cid;
	$uid = $comment_data->uid;
	$subject =$comment_data->subject;
	$created = $comment_data->created;
	$rating = $comment_data->rating;
	$body = $comment_data->body;

	/*Create node review*/
  $node = new stdClass();
  $node->title = $subject;
  $node->type = 'review';
  node_object_prepare($node);
  $node->language = LANGUAGE_NONE;
  $node->uid = $uid;
  $node->body[$node->language][0]['value']   = $body;
  //$node->body[$node->language][0]['summary'] = text_summary($bodytext);
  $node->body[$node->language][0]['format']  = 'full_html';
  $node->field_article[$node->language][]['target_id'] = $nid;
  $node->field_article[$node->language][]['target_type'] = 'node';
  $node->field_review_rating[$node->language][]['rating'] = $rating;
  $node = node_submit($node);
  node_save($node);

  drupal_set_message('when finished, this will convert the comment ' . $cid . ' which has the subject ' . $subject . ' and body ' . $body .' and gives a rate of ' . $rating . ' to the node ' . $nid);

  drupal_set_message('Review with nid ' . $node->nid . ' saved');
}
	}
