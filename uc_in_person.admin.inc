<?php
/**
 * Store an in-person payment for an order and put in a payment date.
 */
function uc_in_person_process_payment_form($form_state, $order) {
  $balance = uc_payment_balance($order);
  $context = array(
    'revision' => 'formatted-original',
    'type' => 'amount',
  );
  $options = array(
    'sign' => FALSE,
    'thou' => FALSE,
    'dec' => '.',
  );
  $form['balance'] = array('#value' => uc_price($balance, $context));
  $form['order_id'] = array(
    '#type' => 'hidden',
    '#value' => $order->order_id,
  );
  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount'),
    '#default_value' => uc_price($balance, $context, $options),
    '#size' => 10,
    '#field_prefix' => variable_get('uc_sign_after_amount', FALSE) ? '' : variable_get('uc_currency_sign', '$'),
    '#field_suffix' => variable_get('uc_sign_after_amount', FALSE) ? variable_get('uc_currency_sign', '$') : '',
  );
  $form['comment'] = array(
    '#type' => 'textfield',
    '#title' => t('Comment'),
    '#description' => t('Any notes about the payment.'),
    '#size' => 64,
    '#maxlength' => 256,
  );
  $form['payment'] = array(
    '#type' => 'fieldset',
    '#title' => t('Payment date'),
    '#collapsible' => FALSE,
  );
  $form['payment']['payment_month'] = uc_select_month(NULL, format_date(time(), 'custom', 'n'));
  $form['payment']['payment_day'] = uc_select_day(NULL, format_date(time(), 'custom', 'j'));
  $form['payment']['payment_year'] = uc_select_year(NULL, format_date(time(), 'custom', 'Y'), format_date(time(), 'custom', 'Y'), format_date(time(), 'custom', 'Y') + 1);
  foreach (array('payment_month', 'payment_day', 'payment_year') as $key) {
    $form['payment'][$key]['#prefix'] = '<div style="float: left; margin-right: 1em;">';
    $form['payment'][$key]['#suffix'] = '</div>';
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Process Payment'),
  );
  return $form;
}

function theme_uc_in_person_process_payment_form($form) {
  $output = '<p>'. t('Use this form to enter an in-person payment into the payments system and set the date it was received.') .'</p>';
  $output .= '<p><strong>'. t('Order balance:') .'</strong> '. drupal_render($form['balance']) .'</p>';
  $output .= drupal_render($form);
  return $output;
}

function uc_in_person_process_payment_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['amount'])) {
    form_set_error('amount', t('The amount must be a number.'));
  }
}

function uc_in_person_process_payment_form_submit($form, &$form_state) {
  global $user;

  uc_payment_enter($form_state['values']['order_id'], 'in_person', $form_state['values']['amount'], $user->uid, '', $form_state['values']['comment']);

  db_query("INSERT INTO {uc_payment_in_person} (order_id, payment_date) VALUES (%d, %d)",
    $form_state['values']['order_id'], mktime(12, 0, 0, $form_state['values']['payment_month'],
    $form_state['values']['payment_day'], $form_state['values']['payment_year']));

  drupal_set_message(t('Payment has been recorded as received on @date.', array('@date' => $form_state['values']['payment_month'] .'/'. $form_state['values']['payment_day'] .'/'. $form_state['values']['payment_year'])));

  $form_state['redirect'] = 'admin/store/orders/'. $form_state['values']['order_id'];
}
