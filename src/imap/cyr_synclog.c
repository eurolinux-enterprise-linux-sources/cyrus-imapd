/* cyr_synclog.c -- add a line to the sync log file for replication
 *
 * Copyright (c) 1994-2008 Carnegie Mellon University.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The name "Carnegie Mellon University" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For permission or any legal
 *    details, please contact
 *      Carnegie Mellon University
 *      Center for Technology Transfer and Enterprise Creation
 *      4615 Forbes Avenue
 *      Suite 302
 *      Pittsburgh, PA  15213
 *      (412) 268-7393, fax: (412) 268-7395
 *      innovation@andrew.cmu.edu
 *
 * 4. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by Computing Services
 *     at Carnegie Mellon University (http://www.cmu.edu/computing/)."
 *
 * CARNEGIE MELLON UNIVERSITY DISCLAIMS ALL WARRANTIES WITH REGARD TO
 * THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS, IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY BE LIABLE
 * FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * $Id: cyr_synclog.c,v 1.7 2010/01/06 17:01:31 murch Exp $
 *
 * Originally written by Bron Gondwana <brong@fastmail.fm>
 */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "global.h"
#include "sync_log.h"
#include "util.h"
#include "xmalloc.h"

/* config.c stuff */
const int config_need_data = 0;

int main(int argc, char *argv[])
{
    char *alt_config = NULL;
    char cmd = '\0';
    char opt;

    if ((geteuid()) == 0 && (become_cyrus() != 0)) {
	fatal("must run as the Cyrus user", EC_USAGE);
    }

    while ((opt = getopt(argc, argv, "C:uvmacqnsb")) != EOF) {
	switch (opt) {
	case 'C': /* alt config file */
	    alt_config = optarg;
	    break;
	case 'u': /* User */
	    cmd = 'u';
	    break;
	case 'v': /* sieVe */
	    cmd = 'v';
	    break;
	case 'm': /* Mailbox */
	    cmd = 'm';
	    break;
	case 'a': /* Append */
	    cmd = 'a';
	    break;
	case 'c': /* aCl */
	    cmd = 'c';
	    break;
	case 'q': /* Quota */
	    cmd = 'q';
	    break;
	case 'n': /* aNnotation */
	    cmd = 'n';
	    break;
	case 's': /* Seen */
	    cmd = 's';
	    break;
	case 'b': /* suBscription */
	    cmd = 'b';
	    break;
	}
    }

    if((argc - optind) < 1) {
	fprintf(stderr, "Usage: %s [-C altconfig] [-{type}] value\n", argv[0]);
	
	fprintf(stderr, "\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "types:\n");
	fprintf(stderr, "  -u   USER\n");
	fprintf(stderr, "  -v   SIEVE\n");
	fprintf(stderr, "  -m   MAILBOX\n");
	fprintf(stderr, "  -q   QUOTA\n");
	fprintf(stderr, "  -n   ANNOTATION\n");
	fprintf(stderr, "  -s   SEEN\n");
	fprintf(stderr, "  -b   SUBSCRIPTION\n");
	fprintf(stderr, "\n");
	fprintf(stderr,
		"You may omit the type flag and just specify a complete log line\n");
	exit(-1);
    }

    cyrus_init(alt_config, "cyr_synclog", 0);
    sync_log_init();

    switch(cmd) {
	case 'u': /* User */
	    sync_log_user(argv[optind]);
	    break;
	case 'v': /* sieVe */
	    sync_log_sieve(argv[optind]);
	    break;
	case 'm': /* Mailbox */
	    sync_log_mailbox(argv[optind]);
	    break;
	case 'q': /* Quota */
	    sync_log_quota(argv[optind]);
	    break;
	case 'n': /* aNnotation */
            sync_log_annotation(argv[optind]);
	    break;
	case 's': /* Seen */
	    sync_log_seen(argv[optind], argv[optind+1]);
	    break;
	case 'b': /* suBscription */
	    sync_log_subscribe(argv[optind], argv[optind+1]);
	    break;
        default:
            /* just as is! */
            sync_log(argv[optind]);
            break;
    }

    sync_log_done();

    cyrus_done();

    return 0;
}
