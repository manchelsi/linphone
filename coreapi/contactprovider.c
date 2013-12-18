/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "contactprovider.h"
#include <linphonecore.h>

/* LinphoneContactSearchRequest
 */

void linphone_contact_search_init(LinphoneContactSearch* obj,
								const char* predicate,
								ContactSearchCallback cb,
								void* cb_data)
{
	static unsigned int request_id_counter = 1;
	obj->id = request_id_counter++; // unique id
	obj->predicate = ms_strdup(predicate?predicate:"");
	obj->cb   = cb;
	obj->data = cb_data;
}

static void linphone_contact_search_destroy( LinphoneContactSearch* req) {
	if( req->predicate ) ms_free(req->predicate);
}

ContactSearchID linphone_contact_search_get_id(LinphoneContactSearch* obj)
{
	return obj->id;
}

const char*linphone_contact_search_get_predicate(LinphoneContactSearch* obj)
{
	return obj->predicate;
}

void linphone_contact_search_invoke_cb(LinphoneContactSearch* req, MSList* friends)
{
	if( req->cb ) req->cb(req, friends, req->data);
}

int linphone_contact_search_compare(const void* a, const void* b) {
	LinphoneContactSearch *ra=((LinphoneContactSearch*)a);
	LinphoneContactSearch *rb=((LinphoneContactSearch*)b);
	return !(ra->id == rb->id); // return 0 if id is equal, 1 otherwise
}

BELLE_SIP_DECLARE_NO_IMPLEMENTED_INTERFACES(LinphoneContactSearch);

BELLE_SIP_INSTANCIATE_VPTR(LinphoneContactSearch,belle_sip_object_t,
	(belle_sip_object_destroy_t)linphone_contact_search_destroy,
	NULL, // clone
	NULL, // marshal
	FALSE
);

/*
 * LinphoneContactProvider
 */


void linphone_contact_provider_init(LinphoneContactProvider* obj, LinphoneCore* lc){
	obj->lc = lc;
}

static void contact_provider_destroy(LinphoneContactProvider* obj){
	(void)obj;
}


BELLE_SIP_DECLARE_NO_IMPLEMENTED_INTERFACES(LinphoneContactProvider);
BELLE_SIP_INSTANCIATE_CUSTOM_VPTR(LinphoneContactProvider)=
{
	{
		BELLE_SIP_VPTR_INIT(LinphoneContactProvider,belle_sip_object_t,TRUE),
		(belle_sip_object_destroy_t) contact_provider_destroy,
		NULL,/*no clone*/
		NULL,/*no marshal*/
	},
	"",
	// Pure virtual
	NULL, /* begin_search -> pure virtual */
	NULL  /* cancel_search -> pure virtual */
};


