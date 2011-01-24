In Person Payment Method README

The In-Person payment method supplied here can be used to allow the customer to complete checkout without immediately submitting a payment. The order is marked as pending, until an administrator goes to the order detail screen and clicks the "process payment" link.

At this point the administrator can add a full or partial payment to the order. When the payment is full, the order is marked as paid.

The workflow is almost identical to the check payment method made available in uc_payment_pack. This module, however, allows the payment method name on the checkout and review pages (they're different) to be configured, as well as the "payment policy", which is the text displayed below the name on the checkout page.

Known issues:
If the uc_credit module is enabled but not configured, the uc_in_person payment method does not show up on the checkout page.
