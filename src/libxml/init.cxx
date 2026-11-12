/*
 * Copyright (C) 2001-2003 Peter J Jones (pjones@pmade.org)
 * All Rights Reserved
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of the Author nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */


// xmlwrapp includes
#include "xmlwrapp/init.h"
#include "utility.h"

// libxml includes
#include <libxml/globals.h>
#include <libxml/xmlerror.h>
#include <libxml/parser.h>

namespace
{

// The parser flags are passed to each parser context as options instead of
// being stored in libxml2 global variables, which are deprecated since libxml2
// 2.12. They are per-thread, as these global variables are when libxml2 is
// built with threads support.
thread_local bool remove_whitespace_flag = false;
thread_local bool substitute_entities_flag = true;
thread_local bool load_external_subsets_flag = true;
thread_local bool validate_xml_flag = false;

bool change_flag_and_return_old_value(int* flag, bool new_value)
{
    const bool old_value = *flag != 0;
    *flag = new_value ? 1 : 0;
    return old_value;
}

bool change_flag_and_return_old_value(bool& flag, bool new_value)
{
    const bool old_value = flag;
    flag = new_value;
    return old_value;
}

} // anonymous namespace

namespace xml
{

int init::ms_counter = 0;

init::init()
{
    if ( ms_counter++ == 0 )
        init_library();
}


init::~init()
{
    if ( --ms_counter == 0 )
        shutdown_library();
}


void init::init_library()
{
    // init the parser (keeps libxml2 thread safe)
    xmlInitParser();

    // set some libxml global variables
    indent_output(true);
}


void init::shutdown_library()
{
    xmlCleanupParser();
}


bool init::indent_output(bool flag)
{
    return change_flag_and_return_old_value(&xmlIndentTreeOutput, flag);
}


bool init::remove_whitespace(bool flag)
{
    return change_flag_and_return_old_value(remove_whitespace_flag, flag);
}


bool init::substitute_entities(bool flag)
{
    return change_flag_and_return_old_value(substitute_entities_flag, flag);
}


bool init::load_external_subsets(bool flag)
{
    return change_flag_and_return_old_value(load_external_subsets_flag, flag);
}


bool init::validate_xml(bool flag)
{
  return change_flag_and_return_old_value(validate_xml_flag, flag);
}


int impl::get_parser_options()
{
    int options = 0;
    if (remove_whitespace_flag)
        options |= XML_PARSE_NOBLANKS;
    if (substitute_entities_flag)
        options |= XML_PARSE_NOENT;
    if (load_external_subsets_flag)
        options |= XML_PARSE_DTDLOAD;
    if (validate_xml_flag)
        options |= XML_PARSE_DTDVALID;

    return options;
}

} // namespace xml
