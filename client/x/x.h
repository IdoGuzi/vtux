#ifndef _X_H_
#define _X_H_

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <string.h>

#include <xcb/xcb.h>
#include <xcb/randr.h>

#include <X11/X.h>
#include <X11/Xlib.h>

struct leaseManager {
    xcb_connection_t *connection;
    xcb_randr_create_lease_reply_t *leaseReply;
};

int createLease(struct leaseManager *lm);

void revokeLease(struct leaseManager *lm);

#endif /*_X_H_*/
