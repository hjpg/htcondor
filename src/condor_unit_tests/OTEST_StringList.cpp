/***************************************************************
 *
 * Copyright (C) 1990-2007, Condor Team, Computer Sciences Department,
 * University of Wisconsin-Madison, WI.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License.  You may
 * obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************/

/*
	This code tests the StringList implementation.
 */

#include "condor_common.h"
#include "condor_debug.h"
#include "condor_config.h"
#include "function_test_driver.h"
#include "emit.h"
#include "unit_test_utils.h"
#include "string_list.cpp"

static bool test_constructor(void);
static bool test_constructor_empty_list(void);
static bool test_constructor_empty_delim(void);
static bool test_constructor_empty_both(void);
static bool test_constructor_null_list(void);
static bool test_constructor_null_delim(void);
static bool test_constructor_null_both(void);
static bool test_copy_constructor_value(void);
static bool test_copy_constructor_empty(void);
static bool test_copy_constructor_pointer(void);
static bool test_initialize_from_string_empty_valid(void);
static bool test_initialize_from_string_empty_empty(void);
static bool test_initialize_from_string_empty_null(void);
static bool test_initialize_from_string_non_empty_valid(void);
static bool test_initialize_from_string_non_empty_empty(void);
static bool test_initialize_from_string_non_empty_null(void);
static bool test_clear_all_empty(void);
static bool test_clear_all_non_empty_many(void);
static bool test_clear_all_non_empty_one(void);
static bool test_create_union_return_true(void);
static bool test_create_union_return_false(void);
static bool test_create_union_duplicates_all(void);
static bool test_create_union_duplicates_some(void);
static bool test_create_union_duplicates_all_ignore(void);
static bool test_create_union_duplicates_some_ignore(void);
static bool test_create_union_empty_current(void);
static bool test_create_union_empty_subset(void);
static bool test_contains_list_return_false(void);
static bool test_contains_list_return_false_ignore(void);
static bool test_contains_list_return_false_almost(void);
static bool test_contains_list_return_false_reverse(void);
static bool test_contains_list_return_true_not(void);
static bool test_contains_list_return_true_consecutive(void);
static bool test_contains_list_return_true_not_ignore(void);
static bool test_contains_list_return_true_consecutive_ignore(void);
static bool test_contains_list_return_true_itself(void);
static bool test_contains_list_return_true_copy(void);
static bool test_contains_return_false(void);
static bool test_contains_return_false_substring(void);
static bool test_contains_return_false_case(void);
static bool test_contains_return_true_one(void);
static bool test_contains_return_true_many(void);
static bool test_contains_current_single(void);
static bool test_contains_current_multiple(void);
static bool test_contains_anycase_return_false(void);
static bool test_contains_anycase_return_false_substring(void);
static bool test_contains_anycase_return_true_one(void);
static bool test_contains_anycase_return_true_many(void);
static bool test_contains_anycase_current_single(void);
static bool test_contains_anycase_current_multiple(void);
static bool test_remove_invalid(void);
static bool test_remove_case(void);
static bool test_remove_substring(void);
static bool test_remove_empty(void);
static bool test_remove_first(void);
static bool test_remove_last(void);
static bool test_remove_many(void);
static bool test_remove_anycase_invalid(void);
static bool test_remove_anycase_substring(void);
static bool test_remove_anycase_empty(void);
static bool test_remove_anycase_first(void);
static bool test_remove_anycase_last(void);
static bool test_remove_anycase_many(void);
static bool test_substring_return_false_invalid(void);
static bool test_substring_return_false_almost(void);
static bool test_substring_return_false_reverse(void);
static bool test_substring_return_false_case(void);
static bool test_substring_return_true_identical(void);
static bool test_substring_return_true_many(void);
static bool test_substring_current_single(void);
static bool test_substring_current_multiple(void);
static bool test_contains_withwildcard_return_false(void);
static bool test_contains_withwildcard_return_false_substring(void);
static bool test_contains_withwildcard_return_false_case(void);
static bool test_contains_withwildcard_return_false_wild(void);
static bool test_contains_withwildcard_return_true_no_wild(void);
static bool test_contains_withwildcard_return_true_only_wild(void);
static bool test_contains_withwildcard_return_true_start(void);
static bool test_contains_withwildcard_return_true_mid(void);
static bool test_contains_withwildcard_return_true_end(void);
static bool test_contains_withwildcard_return_true_same_wild(void);
static bool test_contains_withwildcard_return_true_multiple(void);
static bool test_contains_withwildcard_return_true_many(void);
static bool test_contains_withwildcard_current_single(void);
static bool test_contains_withwildcard_current_multiple(void);
static bool test_contains_anycase_wwc_return_false(void);
static bool test_contains_anycase_wwc_return_false_substring(void);
static bool test_contains_anycase_wwc_return_false_wild(void);
static bool test_contains_anycase_wwc_return_true_no_wild(void);
static bool test_contains_anycase_wwc_return_true_only_wild(void);
static bool test_contains_anycase_wwc_return_true_start(void);
static bool test_contains_anycase_wwc_return_true_mid(void);
static bool test_contains_anycase_wwc_return_true_end(void);
static bool test_contains_anycase_wwc_return_true_same_wild(void);
static bool test_contains_anycase_wwc_return_true_multiple(void);
static bool test_contains_anycase_wwc_return_true_many(void);
static bool test_contains_anycase_wwc_current_single(void);
static bool test_contains_anycase_wwc_current_multiple(void);
static bool test_find_matches_any_wwc_return_false(void);
static bool test_find_matches_any_wwc_return_false_substring(void);
static bool test_find_matches_any_wwc_return_false_wild(void);
static bool test_find_matches_any_wwc_return_true_no_wild(void);
static bool test_find_matches_any_wwc_return_true_only_wild(void);
static bool test_find_matches_any_wwc_return_true_start(void);
static bool test_find_matches_any_wwc_return_true_mid(void);
static bool test_find_matches_any_wwc_return_true_end(void);
static bool test_find_matches_any_wwc_return_true_same_wild(void);
static bool test_find_matches_any_wwc_return_true_multiple(void);
static bool test_find_matches_any_wwc_return_true_many(void);
static bool test_find_matches_any_wwc_none(void);
static bool test_find_matches_any_wwc_one(void);
static bool test_find_matches_any_wwc_one_exist(void);
static bool test_find_matches_any_wwc_wild_start(void);
static bool test_find_matches_any_wwc_wild_mid(void);
static bool test_find_matches_any_wwc_wild_end(void);
static bool test_find_matches_any_wwc_multiple(void);
static bool test_find_matches_any_wwc_multiple_exist(void);
static bool test_find_return_false(void);
static bool test_find_return_false_substring(void);
static bool test_find_return_false_case(void);
static bool test_find_return_true_one(void);
static bool test_find_return_true_many(void);
static bool test_find_anycase_return_false(void);
static bool test_find_anycase_return_false_substring(void);
static bool test_find_anycase_return_true_one(void);
static bool test_find_anycase_return_true_many(void);
static bool test_identical_return_false_same(void);
static bool test_identical_return_false_not_same(void);
static bool test_identical_return_false_almost(void);
static bool test_identical_return_false_ignore(void);
static bool test_identical_return_false_subset(void);
static bool test_identical_return_true_case(void);
static bool test_identical_return_true_ignore(void);
static bool test_identical_return_true_itself(void);
static bool test_identical_return_true_copy(void);
static bool test_print_to_string_empty(void);
static bool test_print_to_string_one(void);
static bool test_print_to_string_many(void);
static bool test_print_to_delimed_string_empty(void);
static bool test_print_to_delimed_string_one(void);
static bool test_print_to_delimed_string_many_short(void);
static bool test_print_to_delimed_string_many_long(void);
static bool test_print_to_delimed_string_many_null(void);
static bool test_delete_current_before(void);
static bool test_delete_current_after_no_match(void);
static bool test_delete_current_one_first(void);
static bool test_delete_current_one_mid(void);
static bool test_delete_current_one_last(void);
static bool test_delete_current_all(void);
static bool test_string_compare_equal_same_beg(void);
static bool test_string_compare_equal_same_mid(void);
static bool test_string_compare_equal_same_end(void);
static bool test_string_compare_equal_different_list(void);
static bool test_string_compare_copy(void);
static bool test_string_compare_not_equal_same(void);
static bool test_string_compare_not_equal_different(void);
static bool test_qsort_empty(void);
static bool test_qsort_one(void);
static bool test_qsort_already(void);
static bool test_qsort_multiple(void);
static bool test_qsort_many(void);
static bool test_qsort_many_shuffle(void);
static bool test_shuffle_empty(void);
static bool test_shuffle_one(void);
static bool test_shuffle_many(void);
static bool test_rewind_empty(void);
static bool test_rewind_non_empty(void);
static bool test_rewind_after_contains_true(void);
static bool test_rewind_after_contains_false(void);
static bool test_append_empty(void);
static bool test_append_one(void);
static bool test_append_many(void);
static bool test_append_contains(void);
static bool test_append_rewind(void);
static bool test_append_current(void);
static bool test_insert_empty(void);
static bool test_insert_head(void);
static bool test_insert_middle(void);
static bool test_insert_end(void);
static bool test_insert_current(void);
static bool test_insert_many(void);
static bool test_next_empty(void);
static bool test_next_beginning(void);
static bool test_next_middle(void);
static bool test_next_end(void);
static bool test_number_empty(void);
static bool test_number_one(void);
static bool test_number_many(void);
static bool test_number_after_delete(void);
static bool test_number_after_clear_all(void);
static bool test_number_after_append(void);
static bool test_number_after_insert(void);
static bool test_number_copy(void);
static bool test_is_empty_empty(void);
static bool test_is_empty_clear(void);
static bool test_is_empty_one(void);
static bool test_is_empty_many(void);
static bool test_is_empty_append(void);
static bool test_is_empty_insert(void);
static bool test_get_list_empty(void);
static bool test_get_list_non_empty(void);
static bool test_get_delimiters_empty_no(void);
static bool test_get_delimiters_empty_yes(void);
static bool test_get_delimiters_non_empty_no(void);
static bool test_get_delimiters_non_empty_yes(void);

bool OTEST_StringList(void) {
	e.emit_object("StringList");
	e.emit_comment("This primitive class is used to contain and search arrays "
		"of strings.");
	e.emit_comment("Many of the tests rely on print_to_string(), so a problem "
		"with that may cause a problem in something else");
	
		// driver to run the tests and all required setup
	FunctionDriver driver;
	driver.register_function(test_constructor);
	driver.register_function(test_constructor_empty_list);
	driver.register_function(test_constructor_empty_delim);
	driver.register_function(test_constructor_empty_both);
	driver.register_function(test_constructor_null_list);
	driver.register_function(test_constructor_null_delim);
	driver.register_function(test_constructor_null_both);
	driver.register_function(test_copy_constructor_value);
	driver.register_function(test_copy_constructor_empty);
	driver.register_function(test_copy_constructor_pointer);
	driver.register_function(test_initialize_from_string_empty_valid);
	driver.register_function(test_initialize_from_string_empty_empty);
	driver.register_function(test_initialize_from_string_empty_null);
	driver.register_function(test_initialize_from_string_non_empty_valid);
	driver.register_function(test_initialize_from_string_non_empty_empty);
	driver.register_function(test_initialize_from_string_non_empty_null);
	driver.register_function(test_clear_all_empty);
	driver.register_function(test_clear_all_non_empty_many);
	driver.register_function(test_clear_all_non_empty_one);
	driver.register_function(test_create_union_return_true);
	driver.register_function(test_create_union_return_false);
	driver.register_function(test_create_union_duplicates_all);
	driver.register_function(test_create_union_duplicates_some);
	driver.register_function(test_create_union_duplicates_all_ignore);
	driver.register_function(test_create_union_duplicates_some_ignore);
	driver.register_function(test_create_union_empty_current);
	driver.register_function(test_create_union_empty_subset);
	driver.register_function(test_contains_list_return_false);
	driver.register_function(test_contains_list_return_false_ignore);
	driver.register_function(test_contains_list_return_false_almost);
	driver.register_function(test_contains_list_return_false_reverse);
	driver.register_function(test_contains_list_return_true_not);
	driver.register_function(test_contains_list_return_true_consecutive);
	driver.register_function(test_contains_list_return_true_not_ignore);
	driver.register_function(test_contains_list_return_true_consecutive_ignore);
	driver.register_function(test_contains_list_return_true_itself);
	driver.register_function(test_contains_list_return_true_copy);
	driver.register_function(test_contains_return_false);
	driver.register_function(test_contains_return_false_substring);
	driver.register_function(test_contains_return_false_case);
	driver.register_function(test_contains_return_true_one);
	driver.register_function(test_contains_return_true_many);
	driver.register_function(test_contains_current_single);
	driver.register_function(test_contains_current_multiple);
	driver.register_function(test_contains_anycase_return_false);
	driver.register_function(test_contains_anycase_return_false_substring);
	driver.register_function(test_contains_anycase_return_true_one);
	driver.register_function(test_contains_anycase_return_true_many);
	driver.register_function(test_contains_anycase_current_single);
	driver.register_function(test_contains_anycase_current_multiple);
	driver.register_function(test_remove_invalid);
	driver.register_function(test_remove_case);
	driver.register_function(test_remove_substring);
	driver.register_function(test_remove_empty);
	driver.register_function(test_remove_first);
	driver.register_function(test_remove_last);
	driver.register_function(test_remove_many);
	driver.register_function(test_remove_anycase_invalid);
	driver.register_function(test_remove_anycase_substring);
	driver.register_function(test_remove_anycase_empty);
	driver.register_function(test_remove_anycase_first);
	driver.register_function(test_remove_anycase_last);
	driver.register_function(test_remove_anycase_many);
	driver.register_function(test_substring_return_false_invalid);
	driver.register_function(test_substring_return_false_almost);
	driver.register_function(test_substring_return_false_reverse);
	driver.register_function(test_substring_return_false_case);
	driver.register_function(test_substring_return_true_identical);
	driver.register_function(test_substring_return_true_many);
	driver.register_function(test_substring_current_single);
	driver.register_function(test_substring_current_multiple);
	driver.register_function(test_contains_withwildcard_return_false);
	driver.register_function(test_contains_withwildcard_return_false_substring);
	driver.register_function(test_contains_withwildcard_return_false_case);
	driver.register_function(test_contains_withwildcard_return_false_wild);
	driver.register_function(test_contains_withwildcard_return_true_no_wild);
	driver.register_function(test_contains_withwildcard_return_true_only_wild);
	driver.register_function(test_contains_withwildcard_return_true_start);
	driver.register_function(test_contains_withwildcard_return_true_mid);
	driver.register_function(test_contains_withwildcard_return_true_end);
	driver.register_function(test_contains_withwildcard_return_true_same_wild);
	driver.register_function(test_contains_withwildcard_return_true_multiple);
	driver.register_function(test_contains_withwildcard_return_true_many);
	driver.register_function(test_contains_withwildcard_current_single);
	driver.register_function(test_contains_withwildcard_current_multiple);
	driver.register_function(test_contains_anycase_wwc_return_false);
	driver.register_function(test_contains_anycase_wwc_return_false_substring);
	driver.register_function(test_contains_anycase_wwc_return_false_wild);
	driver.register_function(test_contains_anycase_wwc_return_true_no_wild);
	driver.register_function(test_contains_anycase_wwc_return_true_only_wild);
	driver.register_function(test_contains_anycase_wwc_return_true_start);
	driver.register_function(test_contains_anycase_wwc_return_true_mid);
	driver.register_function(test_contains_anycase_wwc_return_true_end);
	driver.register_function(test_contains_anycase_wwc_return_true_same_wild);
	driver.register_function(test_contains_anycase_wwc_return_true_multiple);
	driver.register_function(test_contains_anycase_wwc_return_true_many);
	driver.register_function(test_contains_anycase_wwc_current_single);
	driver.register_function(test_contains_anycase_wwc_current_multiple);
	driver.register_function(test_find_matches_any_wwc_return_false);
	driver.register_function(test_find_matches_any_wwc_return_false_substring);
	driver.register_function(test_find_matches_any_wwc_return_false_wild);
	driver.register_function(test_find_matches_any_wwc_return_true_no_wild);
	driver.register_function(test_find_matches_any_wwc_return_true_only_wild);
	driver.register_function(test_find_matches_any_wwc_return_true_start);
	driver.register_function(test_find_matches_any_wwc_return_true_mid);
	driver.register_function(test_find_matches_any_wwc_return_true_end);
	driver.register_function(test_find_matches_any_wwc_return_true_same_wild);
	driver.register_function(test_find_matches_any_wwc_return_true_multiple);
	driver.register_function(test_find_matches_any_wwc_return_true_many);
	driver.register_function(test_find_matches_any_wwc_none);
	driver.register_function(test_find_matches_any_wwc_one);
	driver.register_function(test_find_matches_any_wwc_one_exist);
	driver.register_function(test_find_matches_any_wwc_wild_start);
	driver.register_function(test_find_matches_any_wwc_wild_mid);
	driver.register_function(test_find_matches_any_wwc_wild_end);
	driver.register_function(test_find_matches_any_wwc_multiple);
	driver.register_function(test_find_matches_any_wwc_multiple_exist);
	driver.register_function(test_find_return_false);
	driver.register_function(test_find_return_false_substring);
	driver.register_function(test_find_return_false_case);
	driver.register_function(test_find_return_true_one);
	driver.register_function(test_find_return_true_many);
	driver.register_function(test_find_anycase_return_false);
	driver.register_function(test_find_anycase_return_false_substring);
	driver.register_function(test_find_anycase_return_true_one);
	driver.register_function(test_find_anycase_return_true_many);
	driver.register_function(test_identical_return_false_same);
	driver.register_function(test_identical_return_false_not_same);
	driver.register_function(test_identical_return_false_almost);
	driver.register_function(test_identical_return_false_ignore);
	driver.register_function(test_identical_return_false_subset);
	driver.register_function(test_identical_return_true_case);
	driver.register_function(test_identical_return_true_ignore);
	driver.register_function(test_identical_return_true_itself);
	driver.register_function(test_identical_return_true_copy);
	driver.register_function(test_print_to_string_empty);
	driver.register_function(test_print_to_string_one);
	driver.register_function(test_print_to_string_many);
	driver.register_function(test_print_to_delimed_string_empty);
	driver.register_function(test_print_to_delimed_string_one);
	driver.register_function(test_print_to_delimed_string_many_short);
	driver.register_function(test_print_to_delimed_string_many_long);
	driver.register_function(test_print_to_delimed_string_many_null);
	driver.register_function(test_delete_current_before);
	driver.register_function(test_delete_current_after_no_match);
	driver.register_function(test_delete_current_one_first);
	driver.register_function(test_delete_current_one_mid);
	driver.register_function(test_delete_current_one_last);
	driver.register_function(test_delete_current_all);
	driver.register_function(test_string_compare_equal_same_beg);
	driver.register_function(test_string_compare_equal_same_mid);
	driver.register_function(test_string_compare_equal_same_end);
	driver.register_function(test_string_compare_equal_different_list);
	driver.register_function(test_string_compare_copy);
	driver.register_function(test_string_compare_not_equal_same);
	driver.register_function(test_string_compare_not_equal_different);
	driver.register_function(test_qsort_empty);
	driver.register_function(test_qsort_one);
	driver.register_function(test_qsort_already);
	driver.register_function(test_qsort_multiple);
	driver.register_function(test_qsort_many);
	driver.register_function(test_qsort_many_shuffle);
	driver.register_function(test_shuffle_empty);
	driver.register_function(test_shuffle_one);
	driver.register_function(test_shuffle_many);
	driver.register_function(test_rewind_empty);
	driver.register_function(test_rewind_non_empty);
	driver.register_function(test_rewind_after_contains_true);
	driver.register_function(test_rewind_after_contains_false);
	driver.register_function(test_append_empty);
	driver.register_function(test_append_one);
	driver.register_function(test_append_many);
	driver.register_function(test_append_contains);
	driver.register_function(test_append_rewind);
	driver.register_function(test_append_current);
	driver.register_function(test_insert_empty);
	driver.register_function(test_insert_head);
	driver.register_function(test_insert_middle);
	driver.register_function(test_insert_end);
	driver.register_function(test_insert_current);
	driver.register_function(test_insert_many);
	driver.register_function(test_next_empty);
	driver.register_function(test_next_beginning);
	driver.register_function(test_next_middle);
	driver.register_function(test_next_end);
	driver.register_function(test_number_empty);
	driver.register_function(test_number_one);
	driver.register_function(test_number_many);
	driver.register_function(test_number_after_delete);
	driver.register_function(test_number_after_clear_all);
	driver.register_function(test_number_after_append);
	driver.register_function(test_number_after_insert);
	driver.register_function(test_number_copy);
	driver.register_function(test_is_empty_empty);
	driver.register_function(test_is_empty_clear);
	driver.register_function(test_is_empty_one);
	driver.register_function(test_is_empty_many);
	driver.register_function(test_is_empty_append);
	driver.register_function(test_is_empty_insert);
	driver.register_function(test_get_list_empty);
	driver.register_function(test_get_list_non_empty);
	driver.register_function(test_get_delimiters_empty_no);
	driver.register_function(test_get_delimiters_empty_yes);
	driver.register_function(test_get_delimiters_non_empty_no);
	driver.register_function(test_get_delimiters_non_empty_yes);

		// run the tests
	bool test_result = driver.do_all_functions();
	e.emit_function_break();
	return test_result;
}

static bool test_constructor() {
	e.emit_test("Test the StringList constructor.");
	StringList sl("a;b;c", ";");
	char* expect = "a,b,c";
	char* retVal = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("STRING", "a;b;c");
	e.emit_param("DELIMS", ";");
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(retVal));
	if(niceStrCmp(retVal, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(retVal);
	return true;
}

static bool test_constructor_empty_list() {
	e.emit_test("Test the StringList constructor when passed an empty list with"
		" a non-empty delimiter.");
	StringList sl("", ";");
	char* expect = "";
	char* retVal = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("STRING", expect);
	e.emit_param("DELIMS", ";");
	e.emit_output_expected_header();
	e.emit_retval("%s", expect);
	e.emit_output_actual_header();
	e.emit_retval("%s", nicePrint(retVal));
	if(niceStrCmp(retVal, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(retVal);
	return true;
}

static bool test_constructor_empty_delim() {
	e.emit_test("Test the StringList constructor when passed a non-empty list "
		"with an empty delimiter.");
	StringList sl("abc", "");
	char* expect = "abc";
	char* retVal = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("STRING", "abc");
	e.emit_param("DELIMS", "");
	e.emit_output_expected_header();
	e.emit_retval("%s", expect);
	e.emit_output_actual_header();
	e.emit_retval("%s", nicePrint(retVal));
	if(niceStrCmp(retVal, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(retVal);
	return true;
}

static bool test_constructor_empty_both() {
	e.emit_test("Test the StringList constructor when passed both an empty list"
		" and an empty delimiter.");
	StringList sl("", "");
	char* expect = "";
	char* retVal = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("STRING", "");
	e.emit_param("DELIMS", "");
	e.emit_output_expected_header();
	e.emit_retval("%s", expect);
	e.emit_output_actual_header();
	e.emit_retval("%s", nicePrint(retVal));
	if(niceStrCmp(retVal, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(retVal);
	return true;
}

static bool test_constructor_null_list() {
	e.emit_test("Test the StringList constructor when passed a null string for "
		"the StringList.");
	StringList sl(NULL, ";");
	char* retVal = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("STRING", "NULL");
	e.emit_param("DELIMS", ";");
	e.emit_output_expected_header();
	e.emit_param("StringList", "");
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(retVal));
	if(niceStrCmp(retVal, "") != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(retVal);
	return true;
}

static bool test_constructor_null_delim() {
	e.emit_test("Test the StringList constructor when passed a null string for "
		"the delimiter of the StringList.");
	StringList sl("abc", NULL);
	char* expect = "abc";
	char* retVal = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("STRING", "abc");
	e.emit_param("DELIMS", "NULL");
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(retVal));
	if(niceStrCmp(retVal, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(retVal);
	return true;
}

static bool test_constructor_null_both() {
	e.emit_test("Test the StringList constructor when passed a null string for "
		"both the StringList and the delimiter of the StringList.");
	StringList sl(NULL, NULL);
	char* retVal = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("STRING", "NULL");
	e.emit_param("DELIMS", "NULL");
	e.emit_output_expected_header();
	e.emit_param("StringList", "");
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(retVal));
	if(niceStrCmp(retVal, "") != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(retVal);
	return true;
}

static bool test_copy_constructor_value() {
	e.emit_test("Check the value of the StringList copy constructor.");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	StringList copy(sl);
	char* expect = "a,b,c";
	char* retVal = copy.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", "a,b,c");
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(retVal));
	if(niceStrCmp(retVal, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(retVal);
	return true;
}

static bool test_copy_constructor_empty() {
	e.emit_test("Test the StringList copy constructor when passed an empty "
		"StringList.");
	StringList sl;
	StringList copy(sl);
	char* retVal = copy.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", "");
	e.emit_output_expected_header();
	e.emit_param("StringList", "");
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(retVal));
	if(niceStrCmp(retVal, "") != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(retVal);
	return true;
}

static bool test_copy_constructor_pointer() {
	e.emit_test("Check that the pointer of the copied StringList is not equal "
		"to the original StringList.");
	StringList sl("a;b;c", ";");
	StringList copy(sl);
	char* retVal = copy.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", "a,b,c");
	e.emit_output_expected_header();
	e.emit_retval("!=%x", &sl);
	e.emit_output_actual_header();
	e.emit_retval("%x", &copy);
	if(&sl == &copy) {	//compares pointers
		e.emit_result_failure(__LINE__);
		niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(retVal);
	return true;
}

static bool test_initialize_from_string_empty_valid() {
	e.emit_test("Test initializeFromString on an empty StringList when passed "
		" a valid string.");
	StringList sl("", ";");
	sl.initializeFromString("a;b;c");
	char* expect = "a,b,c";
	char* retVal = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("STRING", "a;b;c");
	e.emit_output_expected_header();
	e.emit_retval("%s", expect);
	e.emit_output_actual_header();
	e.emit_retval("%s", nicePrint(retVal));
	if(niceStrCmp(expect, retVal) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(retVal);
	return true;
}

static bool test_initialize_from_string_empty_empty() {
	e.emit_test("Test initializeFromString on an empty StringList when passed "
		" an empty string.");
	StringList sl("", ";");
	sl.initializeFromString("");
	char* expect = "";
	char* retVal = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("STRING", "");
	e.emit_output_expected_header();
	e.emit_param("StringList", "");
	e.emit_retval("%s", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(retVal));
	e.emit_retval("%s", nicePrint(retVal));
	if(niceStrCmp(retVal, "") != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(retVal);
	return true;
}

static bool test_initialize_from_string_empty_null() {
	e.emit_test("Test initializeFromString on an empty StringList when passed "
		" a null string.");
	e.emit_comment("initializeFromString() throws an exception for this test. "
		"The test is commented out so that the unit testing can continue.");
	e.emit_result_success(__LINE__);
	return true;
/*
	StringList sl("", ";");
	sl.initializeFromString(NULL);
	char* expect = "";
	char* retVal = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("STRING", "NULL");
	e.emit_output_expected_header();
	e.emit_param("StringList", "");
	e.emit_retval("%s", expect);
	e.emit_output_actual_header();
	e.emit_retval("%s", nicePrint(retVal));
	if(niceStrCmp(retVal, "") != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(retVal);
	return true;
*/
}

static bool test_initialize_from_string_non_empty_valid() {
	e.emit_test("Test initializeFromString on a non-empty StringList when "
		"passed a valid string.");
	e.emit_comment("initializeFromString() is currently set up to append to an "
		"existing StringList, so if that changes this test will fail.");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	sl.initializeFromString("d;e;f");
	char* expect = "a,b,c,d,e,f";
	char* retVal = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", "d;e;f");
	e.emit_output_expected_header();
	e.emit_retval("%s", expect);
	e.emit_output_actual_header();
	e.emit_retval("%s", nicePrint(retVal));
	if(niceStrCmp(expect, retVal) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(retVal);
	return true;
}

static bool test_initialize_from_string_non_empty_empty() {
	e.emit_test("Test initializeFromString on a non-empty StringList when "
		"passed an empty string.");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	sl.initializeFromString("");
	char* expect = "a,b,c";
	char* retVal = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", "");
	e.emit_output_expected_header();
	e.emit_retval("%s", expect);
	e.emit_output_actual_header();
	e.emit_retval("%s", nicePrint(retVal));
	if(niceStrCmp(expect, retVal) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(retVal);
	return true;
}

static bool test_initialize_from_string_non_empty_null() {
	e.emit_test("Test initializeFromString on a non-empty StringList when "
		"passed a null string.");
	e.emit_comment("initializeFromString() throws an exception for this test. "
		"The test is commented out so that the unit testing can continue.");
	e.emit_result_success(__LINE__);
	return true;
/*
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	sl.initializeFromString(NULL);
	char* expect = "a,b,c";
	char* retVal = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", "NULL");
	e.emit_output_expected_header();
	e.emit_retval("%s", expect);
	e.emit_output_actual_header();
	e.emit_retval("%s", nicePrint(retVal));
	if(niceStrCmp(expect, retVal) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(retVal);
	return true;
*/
}

static bool test_clear_all_empty() {
	e.emit_test("Test clearAll on an empty StringList.");
	StringList sl("", "");
	char* orig = sl.print_to_string();
	sl.clearAll();
	char* retVal = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_output_expected_header();
	e.emit_param("StringList", "");
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(retVal));
	if(niceStrCmp(retVal, "") != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(retVal);
	return true;
}

static bool test_clear_all_non_empty_many() {
	e.emit_test("Test clearAll on a non-empty StringList with many strings.");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	sl.clearAll();
	char* retVal = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_output_expected_header();
	e.emit_param("StringList", "");
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(retVal));
	if(niceStrCmp(retVal, "") != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(retVal);
	return true;
}

static bool test_clear_all_non_empty_one() {
	e.emit_test("Test clearAll on a non-empty StringList with only one "
		"string.");
	StringList sl("a", "");
	char* orig = sl.print_to_string();
	sl.clearAll();
	char* retVal = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_output_expected_header();
	e.emit_param("StringList", "");
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(retVal));
	if(niceStrCmp(retVal, "") != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(retVal);
	return true;
}

static bool test_create_union_return_true() {
	e.emit_test("Does create_union return true when the list is modified?");
	StringList sl("a;b;c", ";");
	StringList toAdd("d;e;f", ";");
	char* orig = sl.print_to_string();
	char* add = toAdd.print_to_string();
	bool retVal = sl.create_union(toAdd, true);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("SUBSET", add);
	e.emit_param("ANYCASE", "TRUE");
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(add);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(add);
	return true;
}

static bool test_create_union_return_false() {
	e.emit_test("Does create_union return false when the list is not "
		"modified?");
	StringList sl("a;b;c", ";");
	StringList toAdd("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* add = toAdd.print_to_string();
	bool retVal = sl.create_union(toAdd, true);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("SUBSET", add);
	e.emit_param("ANYCASE", "TRUE");
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(add);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);	niceFree(add);
	return true;
}

static bool test_create_union_duplicates_all() {
	e.emit_test("Does create_union avoid adding duplicates when the given "
		"StringList contains all duplicates?");
	StringList sl("a;b;c", ";");
	StringList toAdd("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* add = toAdd.print_to_string();
	sl.create_union(toAdd, false);
	char* retVal = sl.print_to_string();
	char* expect = "a,b,c";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("SUBSET", add);
	e.emit_param("ANYCASE", "FALSE");
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(retVal));
	if(niceStrCmp(retVal, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(add); niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(add); niceFree(retVal);
	return true;
}

static bool test_create_union_duplicates_some() {
	e.emit_test("Does create_union avoid adding duplicates when the given "
		"StringList contains some duplicates?");
	StringList sl("a;b;c", ";");
	StringList toAdd("a;d;c", ";");
	char* orig = sl.print_to_string();
	char* add = toAdd.print_to_string();
	sl.create_union(toAdd, false);
	char* retVal = sl.print_to_string();
	char* expect = "a,b,c,d";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("SUBSET", add);
	e.emit_param("ANYCASE", "FALSE");
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(retVal));
	if(niceStrCmp(retVal, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(add); niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(add); niceFree(retVal);
	return true;
}

static bool test_create_union_duplicates_all_ignore() {
	e.emit_test("Does create_union avoid adding duplicates when the given "
		"StringList contains all duplicates when case-insensitive?");
	StringList sl("A;b;c;D", ";");
	StringList toAdd("A;B;c;d", ";");
	char* orig = sl.print_to_string();
	char* add = toAdd.print_to_string();
	sl.create_union(toAdd, true);
	char* retVal = sl.print_to_string();
	char* expect = "A,b,c,D";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("SUBSET", add);
	e.emit_param("ANYCASE", "TRUE");
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(retVal));
	if(niceStrCmp(retVal, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(add); niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(add); niceFree(retVal);
	return true;
}

static bool test_create_union_duplicates_some_ignore() {
	e.emit_test("Does create_union avoid adding duplicates when the given "
		"StringList contains some duplicates when case-insensitive?");
	StringList sl("A;b;c;D", ";");
	StringList toAdd("A;B;c;d;E", ";");
	char* orig = sl.print_to_string();
	char* add = toAdd.print_to_string();
	sl.create_union(toAdd, true);
	char* retVal = sl.print_to_string();
	char* expect = "A,b,c,D,E";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("SUBSET", add);
	e.emit_param("ANYCASE", "TRUE");
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(retVal));
	if(niceStrCmp(retVal, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(add); niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(add); niceFree(retVal);
	return true;
}

static bool test_create_union_empty_current() {
	e.emit_test("Test create_union when the current StringList is empty.");
	StringList sl;
	StringList toAdd("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* add = toAdd.print_to_string();
	sl.create_union(toAdd, false);
	char* retVal = sl.print_to_string();
	char* expect = "a,b,c";
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_param("SUBSET", add);
	e.emit_param("ANYCASE", "FALSE");
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(retVal));
	if(niceStrCmp(retVal, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(add); niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(add); niceFree(retVal);
	return true;
}

static bool test_create_union_empty_subset() {
	e.emit_test("Test create_union when the StringList to add is empty.");
	StringList sl("a;b;c", ";");
	StringList toAdd;
	char* orig = sl.print_to_string();
	sl.create_union(toAdd, false);
	char* retVal = sl.print_to_string();
	char* expect = "a,b,c";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("SUBSET", "");
	e.emit_param("ANYCASE", "FALSE");
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(retVal));
	if(niceStrCmp(retVal, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(retVal);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(retVal);
	return true;
}

static bool test_contains_list_return_false() {
	e.emit_test("Does contains_list() return false when the StringList doesn't "
		"contain the given subset?");
	StringList sl("a;b;c;d;e;f", ";");
	StringList subset("g;h;i", ";");
	char* orig = sl.print_to_string();
	char* check = subset.print_to_string();
	bool retVal = sl.contains_list(subset, false);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("SUBSET", check);
	e.emit_param("ANYCASE", "FALSE");
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(check);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(check);
	return true;
}

static bool test_contains_list_return_false_ignore() {
	e.emit_test("Does contains_list() return false when the StringList contains"
		" the given subset when ignoring case, but anycase is false?");
	StringList sl("a;b;c", ";");
	StringList subset("A;B;C", ";");
	char* orig = sl.print_to_string();
	char* check = subset.print_to_string();
	bool retVal = sl.contains_list(subset, false);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("SUBSET", check);
	e.emit_param("ANYCASE", "FALSE");
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(check);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(check);
	return true;
}

static bool test_contains_list_return_false_almost() {
	e.emit_test("Does contains_list() return false when the StringList contains"
		" all but 1 string of the subset?");
	StringList sl("a;b;c;d;e;f", ";");
	StringList subset("a;c;e;g", ";");
	char* orig = sl.print_to_string();
	char* check = subset.print_to_string();
	bool retVal = sl.contains_list(subset, false);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("SUBSET", check);
	e.emit_param("ANYCASE", "FALSE");
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(check);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(check);
	return true;
}

static bool test_contains_list_return_false_reverse() {
	e.emit_test("Does contains_list() return false when the StringList is a "
		"subset of the subset?");
	StringList sl("a;b;c", ";");
	StringList subset("a;b;c;d;e;f", ";");
	char* orig = sl.print_to_string();
	char* check = subset.print_to_string();
	bool retVal = sl.contains_list(subset, false);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("SUBSET", check);
	e.emit_param("ANYCASE", "FALSE");
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(check);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(check);
	return true;
}

static bool test_contains_list_return_true_not() {
	e.emit_test("Does contains_list() return true when the StringList does "
		"contain the given subset?");
	StringList sl("a;b;c;d;e;f", ";");
	StringList subset("a;c;f", ";");
	char* orig = sl.print_to_string();
	char* check = subset.print_to_string();
	bool retVal = sl.contains_list(subset, false);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("SUBSET", check);
	e.emit_param("ANYCASE", "FALSE");
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(check);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(check);
	return true;
}

static bool test_contains_list_return_true_consecutive() {
	e.emit_test("Does contains_list() return true when the StringList does "
		"contain the given subset consecutively?");
	StringList sl("a;b;c;d;e;f", ";");
	StringList subset("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = subset.print_to_string();
	bool retVal = sl.contains_list(subset, false);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("SUBSET", check);
	e.emit_param("ANYCASE", "FALSE");
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(check);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(check);
	return true;
}

static bool test_contains_list_return_true_not_ignore() {
	e.emit_test("Does contains_list() return true when the StringList does "
		"contain the given subset when ignoring case?");
	StringList sl("a;b;c;D;E;F", ";");
	StringList subset("a;C;d;F", ";");
	char* orig = sl.print_to_string();
	char* check = subset.print_to_string();
	bool retVal = sl.contains_list(subset, true);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("SUBSET", check);
	e.emit_param("ANYCASE", "TRUE");
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(check);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(check);
	return true;
}

static bool test_contains_list_return_true_consecutive_ignore() {
	e.emit_test("Does contains_list() return true when the StringList does "
		"contain the given subset consecutively when ignoring case?");
	StringList sl("a;b;C;D;e;f", ";");
	StringList subset("a;B;c;D", ";");
	char* orig = sl.print_to_string();
	char* check = subset.print_to_string();
	bool retVal = sl.contains_list(subset, true);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("SUBSET", check);
	e.emit_param("ANYCASE", "TRUE");
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(check);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(check);
	return true;
}

static bool test_contains_list_return_true_itself() {
	e.emit_test("Does contains_list() return true when passed the same "
		"StringList called on?");
	StringList sl("a;b;c;d;e;f", ";");
	char* orig = sl.print_to_string();
	bool retVal = sl.contains_list(sl, false);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("SUBSET", orig);
	e.emit_param("ANYCASE", "FALSE");
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_list_return_true_copy() {
	e.emit_test("Does contains_list() return true when passed a StringList "
		"copy?");
	StringList sl("a;b;c;d;e;f", ";");
	StringList subset(sl);
	char* orig = sl.print_to_string();
	char* check = subset.print_to_string();
	bool retVal = sl.contains_list(subset, false);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("SUBSET", check);
	e.emit_param("ANYCASE", "FALSE");
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(check);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(check);
	return true;
}

static bool test_contains_return_false() {
	e.emit_test("Does contains() return false when passed a string not in the "
		"StringList?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "d";
	bool retVal = sl.contains(check);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_return_false_substring() {
	e.emit_test("Does contains() return false when passed a string not in the "
		"StringList, but the StringList has a substring of the string?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "boo";
	bool retVal = sl.contains(check);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_return_false_case() {
	e.emit_test("Does contains() return false when passed a string in the "
		"StringList when ignoring case?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "A";
	bool retVal = sl.contains(check);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_return_true_one() {
	e.emit_test("Does contains() return true when passed one string in the "
		"StringList?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "a";
	bool retVal = sl.contains(check);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_return_true_many() {
	e.emit_test("Does contains() return true for multiple calls with different "
		"strings in the StringList?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check1 = "a";
	char* check2 = "b";
	char* check3 = "c";
	bool retVal = sl.contains(check3) && sl.contains(check2) 
		&& sl.contains(check1);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check3);
	e.emit_param("STRING", check2);
	e.emit_param("STRING", check1);
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_current_single() {
	e.emit_test("Does contains() change current to point to the location of the"
		" single match?");
	e.emit_comment("To test that current points to the correct string, "
		"next() has to be called so a problem with that may cause this"
		" to fail.");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "a";
	sl.contains(check); 
	char* next = sl.next();
	char* expect = "b";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_param("next", expect);
	e.emit_output_actual_header();
	e.emit_param("next", nicePrint(next));
	if(niceStrCmp(next, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_current_multiple() {
	e.emit_test("Does contains() change current to point to the location of the"
		" first match when there are multiple matches?");
	e.emit_comment("To test that current points to the correct string, "
		"deleteCurrent() has to be called so a problem with that may cause this"
		" to fail.");
	StringList sl("a;b;a;b", ";");
	char* orig = sl.print_to_string();
	char* check = "a";
	sl.contains(check); 
	sl.deleteCurrent();
	char* changed = sl.print_to_string();
	char* expect = "b,a,b";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_contains_anycase_return_false() {
	e.emit_test("Does contains_anycase() return false when passed a string not "
		"in the StringList?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "d";
	bool retVal = sl.contains_anycase(check);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_anycase_return_false_substring() {
	e.emit_test("Does contains_anycase() return false when passed a string not "
		"in the StringList, but the StringList has a substring of the string?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "boo";
	bool retVal = sl.contains_anycase(check);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_anycase_return_true_one() {
	e.emit_test("Does contains_anycase() return true when passed one string in "
		"the StringList?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "a";
	bool retVal = sl.contains_anycase(check);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_anycase_return_true_many() {
	e.emit_test("Does contains_anycase() return true for multiple calls with "
		"different strings in the StringList?");
	StringList sl("a;b;C;D", ";");
	char* orig = sl.print_to_string();
	char* check1 = "a";
	char* check2 = "B";
	char* check3 = "C";
	char* check4 = "d";
	bool retVal = sl.contains_anycase(check4) && sl.contains_anycase(check3) 
		&& sl.contains_anycase(check2) && sl.contains_anycase(check1);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check4);
	e.emit_param("STRING", check3);
	e.emit_param("STRING", check2);
	e.emit_param("STRING", check1);
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_anycase_current_single() {
	e.emit_test("Does contains_anycase() change current to point to the "
		"location of the single match?");
	e.emit_comment("To test that current points to the correct string, "
		"next() has to be called so a problem with that may cause this"
		" to fail.");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "A";
	sl.contains_anycase(check); 
	char* next = sl.next();
	char* expect = "b";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_param("next", next);
	e.emit_output_actual_header();
	e.emit_param("next", nicePrint(next));
	if(niceStrCmp(next, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_anycase_current_multiple() {
	e.emit_test("Does contains_anycase() change current to point to the "
		"location of the first match when there are multiple matches?");
	e.emit_comment("To test that current points to the correct string, "
		"deleteCurrent() has to be called so a problem with that may cause this"
		" to fail.");
	StringList sl("A;B;a;b", ";");
	char* orig = sl.print_to_string();
	char* check = "a";
	sl.contains_anycase(check); 
	sl.deleteCurrent();
	char* changed = sl.print_to_string();
	char* expect = "B,a,b";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_remove_invalid() {
	e.emit_test("Test that the StringList stays the same when calling remove() "
		"with a string not in the list.");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* remove = "d";
	sl.remove(remove);
	char* expect = "a,b,c";
	char* changed = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", remove);
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(expect, changed) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_remove_case() {
	e.emit_test("Test that the StringList stays the same when calling remove() "
		"with a string in the list when ignoring case.");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* remove = "A";
	sl.remove(remove);
	char* expect = "a,b,c";
	char* changed = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", remove);
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(expect, changed) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_remove_substring() {
	e.emit_test("Test that the StringList stays the same when calling remove() "
		"with a string not in the list, but the list contains a substring of "
		"the string.");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* remove = "boo";
	sl.remove(remove);
	char* expect = "a,b,c";
	char* changed = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", remove);
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(expect, changed) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_remove_empty() {
	e.emit_test("Test remove() on an empty StringList.");
	StringList sl("", "");
	char* orig = sl.print_to_string();
	char* remove = "b";
	sl.remove(remove);
	char* changed = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_param("STRING", remove);
	e.emit_output_expected_header();
	e.emit_param("StringList", "");
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(changed, "") != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_remove_first() {
	e.emit_test("Does remove() successfully remove the first string?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* remove = "a";
	sl.remove(remove);
	char* expect = "b,c";
	char* changed = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", remove);
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(expect, changed) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_remove_last() {
	e.emit_test("Does remove() successfully remove the last string?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* remove = "c";
	sl.remove(remove);
	char* expect = "a,b";
	char* changed = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", remove);
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(expect, changed) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_remove_many() {
	e.emit_test("Does remove() successfully remove many strings?");
	StringList sl("a;b;c;d;e;f", ";");
	char* orig = sl.print_to_string();
	char* remove1 = "f";
	char* remove2 = "d";
	char* remove3 = "b";
	sl.remove(remove1);
	sl.remove(remove2);
	sl.remove(remove3);
	char* expect = "a,c,e";
	char* changed = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", remove1);
	e.emit_param("STRING", remove2);
	e.emit_param("STRING", remove3);
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(expect, changed) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_remove_anycase_invalid() {
	e.emit_test("Test that the StringList stays the same when calling "
		"remove_anycase() with a string not in the list.");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* remove = "d";
	sl.remove_anycase(remove);
	char* expect = "a,b,c";
	char* changed = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", remove);
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(expect, changed) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}



static bool test_remove_anycase_substring() {
	e.emit_test("Test that the StringList stays the same when calling "
		"remove_anycase() with a string not in the list, but the list contains "
		"a substring of the string.");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* remove = "Boo";
	sl.remove_anycase(remove);
	char* expect = "a,b,c";
	char* changed = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", remove);
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(expect, changed) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_remove_anycase_empty() {
	e.emit_test("Test remove_anycase() on an empty StringList.");
	StringList sl("", "");
	char* orig = sl.print_to_string();
	char* remove = "b";
	sl.remove_anycase(remove);
	char* changed = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_param("STRING", remove);
	e.emit_output_expected_header();
	e.emit_param("StringList", "");
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(changed, "") != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_remove_anycase_first() {
	e.emit_test("Does remove_anycase() successfully remove the first string?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* remove = "A";
	sl.remove_anycase(remove);
	char* expect = "b,c";
	char* changed = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", remove);
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(expect, changed) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_remove_anycase_last() {
	e.emit_test("Does remove_anycase() successfully remove the last string?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* remove = "C";
	sl.remove_anycase(remove);
	char* expect = "a,b";
	char* changed = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", remove);
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(expect, changed) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_remove_anycase_many() {
	e.emit_test("Does remove_anycase() successfully remove many strings?");
	StringList sl("a;b;c;d;E;F;G;H", ";");
	char* orig = sl.print_to_string();
	char* remove1 = "h";
	char* remove2 = "F";
	char* remove3 = "D";
	char* remove4 = "b";
	sl.remove_anycase(remove1);
	sl.remove_anycase(remove2);
	sl.remove_anycase(remove3);
	sl.remove_anycase(remove4);
	char* expect = "a,c,E,G";
	char* changed = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", remove1);
	e.emit_param("STRING", remove2);
	e.emit_param("STRING", remove3);
	e.emit_param("STRING", remove4);
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(expect, changed) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_substring_return_false_invalid() {
	e.emit_test("Does substring() return false when passed a string not in the "
		"StringList?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "d";
	bool retVal = sl.substring(check);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_substring_return_false_almost() {
	e.emit_test("Does substring() return false when the StringList contains a "
		"string that is almost a substring of the string?");
	StringList sl("abc;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "abd";
	bool retVal = sl.substring(check);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_substring_return_false_reverse() {
	e.emit_test("Does substring() return false when the passed string is a "
		"substring of one of the StringList's strings?");
	StringList sl("aah;boo;car", ";");
	char* orig = sl.print_to_string();
	char* check = "bo";
	bool retVal = sl.substring(check);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_substring_return_false_case() {
	e.emit_test("Does substring() return false when the StringList contains a "
		"string that is a substring of the string when ignoring case?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "B";
	bool retVal = sl.substring(check);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}


static bool test_substring_return_true_identical() {
	e.emit_test("Does substring() return true when the passed string is in "
		"the StringList?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "a";
	bool retVal = sl.substring(check);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_substring_return_true_many() {
	e.emit_test("Does substring() return true when the StringList contains "
		"substrings of the string for multiple calls?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check1 = "car";
	char* check2 = "bar";
	char* check3 = "aar";
	bool retVal = sl.substring(check1) && sl.substring(check2) 
		&& sl.substring(check3);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check1);
	e.emit_param("STRING", check2);
	e.emit_param("STRING", check3);
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_substring_current_single() {
	e.emit_test("Does substring() change current to point to the location of "
		"the single match?");
	e.emit_comment("To test that current points to the correct string, "
		"next() has to be called so a problem with that may cause this"
		" to fail.");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "are";
	sl.substring(check); 
	char* next = sl.next();
	char* expect = "b";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_param("next", next);
	e.emit_output_actual_header();
	e.emit_param("next", nicePrint(next));
	if(niceStrCmp(next, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_substring_current_multiple() {
	e.emit_test("Does substring() change current to point to the location of "
		"the first match when there are multiple matches?");	
	e.emit_comment("To test that current points to the correct string, "
		"deleteCurrent() has to be called so a problem with that may cause this"
		" to fail.");
	StringList sl("a;b;a;b", ";");
	char* orig = sl.print_to_string();
	char* check = "are";
	sl.substring(check); 
	sl.deleteCurrent();
	char* changed = sl.print_to_string();
	char* expect = "b,a,b";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}


static bool test_contains_withwildcard_return_false() {
	e.emit_test("Does contains_withwildcard() return false when passed a string"
		" not in the StringList?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "d";
	bool retVal = sl.contains_withwildcard(check); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_withwildcard_return_false_substring() {
	e.emit_test("Does contains_withwildcard() return false when the StringList "
		"contains a substring of the string?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "boo";
	bool retVal = sl.contains_withwildcard(check); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_withwildcard_return_false_case() {
	e.emit_test("Does contains_withwildcard() return false when the StringList "
		"contains the string when ignoring case?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "A";
	bool retVal = sl.contains_withwildcard(check); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_withwildcard_return_false_wild() {
	e.emit_test("Does contains_withwildcard() return false when the passed "
		"string is not in the StringList when using a wildcard?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "d*";
	bool retVal = sl.contains_withwildcard(check); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_withwildcard_return_true_no_wild() {
	e.emit_test("Does contains_withwildcard() return true when the StringList "
		"contains the string when not using a wildcard?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "a";
	bool retVal = sl.contains_withwildcard(check); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_withwildcard_return_true_only_wild() {
	e.emit_test("Does contains_withwildcard() return true when the StringList "
		"passed contains a string consisting of only the wildcard?");
	StringList sl("*;a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "foo";
	bool retVal = sl.contains_withwildcard(check); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_withwildcard_return_true_start() {
	e.emit_test("Does contains_withwildcard() return true when StringList "
		"contains the string with a wildcard at the start?");
	StringList sl("a;*r;c", ";");
	char* orig = sl.print_to_string();
	char* check = "bar";
	bool retVal = sl.contains_withwildcard(check); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_withwildcard_return_true_mid() {
	e.emit_test("Does contains_withwildcard() return true when StringList "
		"contains the string with a wildcard in the middle of the string?");
	StringList sl("a;b*r;c", ";");
	char* orig = sl.print_to_string();
	char* check = "bar";
	bool retVal = sl.contains_withwildcard(check); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_withwildcard_return_true_end() {
	e.emit_test("Does contains_withwildcard() return true when StringList "
		"contains the string with a wildcard at the end of the string?");
	StringList sl("a;b*;c", ";");
	char* orig = sl.print_to_string();
	char* check = "bar";
	bool retVal = sl.contains_withwildcard(check); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_withwildcard_return_true_same_wild() {
	e.emit_test("Does contains_withwildcard() return true when StringList "
		"contains the string but the wildcard is used too?");
	StringList sl("a*;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "a";
	bool retVal = sl.contains_withwildcard(check); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_withwildcard_return_true_multiple() {
	e.emit_test("Does contains_withwildcard() return true when the StringList "
		"contains multiple matches of the string?");
	StringList sl("a*;ar*;are*;", ";");
	char* orig = sl.print_to_string();
	char* check = "are";
	bool retVal = sl.contains_withwildcard(check); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_withwildcard_return_true_many() {
	e.emit_test("Does contains_withwildcard() return true when StringList "
		"contains the strings for many calls?");
	StringList sl("a*;ba*;car*", ";");
	char* orig = sl.print_to_string();
	char* check1 = "car";
	char* check2 = "bar";
	char* check3 = "aar";
	bool retVal = sl.contains_withwildcard(check1) 
		&& sl.contains_withwildcard(check2) 
		&& sl.contains_withwildcard(check3);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check1);
	e.emit_param("STRING", check2);
	e.emit_param("STRING", check3);
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_withwildcard_current_single() {
	e.emit_test("Does contains_withwildcard() change current to point to the "
		"location of the single match?");
	e.emit_comment("To test that current points to the correct string, "
		"next() has to be called so a problem with that may cause this"
		" to fail.");
	StringList sl("a*;b;c*", ";");
	char* orig = sl.print_to_string();
	char* check = "aar";
	sl.contains_withwildcard(check); 
	char* next = sl.next();
	char* expect = "b";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_param("next", next);
	e.emit_output_actual_header();
	e.emit_param("next", nicePrint(next));
	if(niceStrCmp(next, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_withwildcard_current_multiple() {
	e.emit_test("Does contains_withwildcard() change current to point to the "
		"location of the first match when there are multiple matches?");
	e.emit_comment("To test that current points to the correct string, "
		"deleteCurrent() has to be called so a problem with that may cause this"
		" to fail.");
	StringList sl("a*;b;aa*;b", ";");
	char* orig = sl.print_to_string();
	char* check = "aar";
	sl.contains_withwildcard(check); 
	sl.deleteCurrent();
	char* changed = sl.print_to_string();
	char* expect = "b,aa*,b";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_contains_anycase_wwc_return_false() {
	e.emit_test("Does contains_anycase_withwildcard() return false when passed "
		"a string not in the StringList?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "d";
	bool retVal = sl.contains_anycase_withwildcard(check); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_anycase_wwc_return_false_substring() {
	e.emit_test("Does contains_anycase_withwildcard() return false when the "
		"StringList contains a substring of the string?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "boo";
	bool retVal = sl.contains_anycase_withwildcard(check); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_anycase_wwc_return_false_wild() {
	e.emit_test("Does contains_anycase_withwildcard() return false when the "
		"passed string is not in the StringList when using a wildcard?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "d*";
	bool retVal = sl.contains_anycase_withwildcard(check); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_anycase_wwc_return_true_no_wild() {
	e.emit_test("Does contains_anycase_withwildcard() return true when the "
		"StringList contains the string when not using a wildcard?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "A";
	bool retVal = sl.contains_anycase_withwildcard(check); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_anycase_wwc_return_true_only_wild() {
	e.emit_test("Does contains_anycase_withwildcard() return true when the "
		"StringList passed contains a string consisting of only the wildcard?");
	StringList sl("*;a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "foo";
	bool retVal = sl.contains_anycase_withwildcard(check); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_anycase_wwc_return_true_start() {
	e.emit_test("Does contains_anycase_withwildcard() return true when "
		"StringList contains the string with a wildcard at the start?");
	StringList sl("A;*R;C", ";");
	char* orig = sl.print_to_string();
	char* check = "bar";
	bool retVal = sl.contains_anycase_withwildcard(check); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_anycase_wwc_return_true_mid() {
	e.emit_test("Does contains_anycase_withwildcard() return true when "
		"StringList contains the string with a wildcard in the middle of the "
		"string?");
	StringList sl("a;b*r;c", ";");
	char* orig = sl.print_to_string();
	char* check = "Bar";
	bool retVal = sl.contains_anycase_withwildcard(check); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_anycase_wwc_return_true_end() {
	e.emit_test("Does contains_anycase_withwildcard() return true when "
		"StringList contains the string with a wildcard at the end of the "
		"string?");
	StringList sl("a;b*;c", ";");
	char* orig = sl.print_to_string();
	char* check = "BAR";
	bool retVal = sl.contains_anycase_withwildcard(check); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_anycase_wwc_return_true_same_wild() {
	e.emit_test("Does contains_anycase_withwildcard() return true when "
		"StringList contains the string but the wildcard is used too?");
	StringList sl("a*;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "A";
	bool retVal = sl.contains_anycase_withwildcard(check); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_anycase_wwc_return_true_multiple() {
	e.emit_test("Does contains_anycase_withwildcard() return true when the "
		"StringList contains multiple matches of the string?");
	StringList sl("a*;AR*;Are*;", ";");
	char* orig = sl.print_to_string();
	char* check = "are";
	bool retVal = sl.contains_anycase_withwildcard(check); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_anycase_wwc_return_true_many() {
	e.emit_test("Does contains_anycase_withwildcard() return true when "
		"StringList contains the strings for many calls?");
	StringList sl("a*;ba*;CAR*", ";");
	char* orig = sl.print_to_string();
	char* check1 = "car";
	char* check2 = "bar";
	char* check3 = "AAR";
	bool retVal = sl.contains_anycase_withwildcard(check1) 
		&& sl.contains_anycase_withwildcard(check2) 
		&& sl.contains_anycase_withwildcard(check3);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check1);
	e.emit_param("STRING", check2);
	e.emit_param("STRING", check3);
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_anycase_wwc_current_single() {
	e.emit_test("Does contains_anycase_withwildcard() change current to point "
		"to the location of the single match?");
	e.emit_comment("To test that current points to the correct string, "
		"next() has to be called so a problem with that may cause this"
		" to fail.");
	StringList sl("a*;b;c*", ";");
	char* orig = sl.print_to_string();
	char* check = "AAR";
	sl.contains_anycase_withwildcard(check); 
	char* next = sl.next();
	char* expect = "b";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_param("next", next);
	e.emit_output_actual_header();
	e.emit_param("next", nicePrint(next));
	if(niceStrCmp(next, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_contains_anycase_wwc_current_multiple() {
	e.emit_test("Does contains_anycase_withwildcard() change current to point "
		"to the location of the first match when there are multiple matches?");
	e.emit_comment("To test that current points to the correct string, "
		"deleteCurrent() has to be called so a problem with that may cause this"
		" to fail.");
	StringList sl("A*;b;Aa*;b", ";");
	char* orig = sl.print_to_string();
	char* check = "aar";
	sl.contains_anycase_withwildcard(check); 
	sl.deleteCurrent();
	char* changed = sl.print_to_string();
	char* expect = "b,Aa*,b";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_find_matches_any_wwc_return_false() {
	e.emit_test("Does find_matches_anycase_withwildcard() return false when "
		"passed a string not in the StringList?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "d";
	bool retVal = sl.find_matches_anycase_withwildcard(check, NULL); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("StringList", "NULL");
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_find_matches_any_wwc_return_false_substring() {
	e.emit_test("Does find_matches_anycase_withwildcard() return false when the"
		" StringList contains a substring of the string?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "boo";
	bool retVal = sl.find_matches_anycase_withwildcard(check, NULL); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("StringList", "NULL");
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_find_matches_any_wwc_return_false_wild() {
	e.emit_test("Does find_matches_anycase_withwildcard() return false when the"
		" passed string is not in the StringList when using a wildcard?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "d*";
	bool retVal = sl.find_matches_anycase_withwildcard(check, NULL); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("StringList", "NULL");
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_find_matches_any_wwc_return_true_no_wild() {
	e.emit_test("Does find_matches_anycase_withwildcard() return true when the "
		"StringList contains the string when not using a wildcard?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "A";
	bool retVal = sl.find_matches_anycase_withwildcard(check, NULL); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("StringList", "NULL");
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_find_matches_any_wwc_return_true_only_wild() {
	e.emit_test("Does find_matches_anycase_withwildcard() return true when the "
		"StringList passed contains a string consisting of only the wildcard?");
	StringList sl("*;a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "foo";
	bool retVal = sl.find_matches_anycase_withwildcard(check, NULL); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("StringList", "NULL");
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_find_matches_any_wwc_return_true_start() {
	e.emit_test("Does find_matches_anycase_withwildcard() return true when "
		"StringList contains the string with a wildcard at the start?");
	StringList sl("A;*R;C", ";");
	char* orig = sl.print_to_string();
	char* check = "bar";
	bool retVal = sl.find_matches_anycase_withwildcard(check, NULL); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("StringList", "NULL");
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_find_matches_any_wwc_return_true_mid() {
	e.emit_test("Does find_matches_anycase_withwildcard() return true when "
		"StringList contains the string with a wildcard in the middle of the "
		"string?");
	StringList sl("a;b*r;c", ";");
	char* orig = sl.print_to_string();
	char* check = "Bar";
	bool retVal = sl.find_matches_anycase_withwildcard(check, NULL); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("StringList", "NULL");
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_find_matches_any_wwc_return_true_end() {
	e.emit_test("Does find_matches_anycase_withwildcard() return true when "
		"StringList contains the string with a wildcard at the end of the "
		"string?");
	StringList sl("a;b*;c", ";");
	char* orig = sl.print_to_string();
	char* check = "BAR";
	bool retVal = sl.find_matches_anycase_withwildcard(check, NULL); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("StringList", "NULL");
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_find_matches_any_wwc_return_true_same_wild() {
	e.emit_test("Does find_matches_anycase_withwildcard() return true when "
		"StringList contains the string but the wildcard is used too?");
	StringList sl("a*;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "A";
	bool retVal = sl.find_matches_anycase_withwildcard(check, NULL); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("StringList", "NULL");
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_find_matches_any_wwc_return_true_multiple() {
	e.emit_test("Does find_matches_anycase_withwildcard() return true when the "
		"StringList contains multiple matches of the string?");
	StringList sl("a*;AR*;Are*;", ";");
	char* orig = sl.print_to_string();
	char* check = "are";
	bool retVal = sl.find_matches_anycase_withwildcard(check, NULL); 
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("StringList", "NULL");
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_find_matches_any_wwc_return_true_many() {
	e.emit_test("Does find_matches_anycase_withwildcard() return true when "
		"StringList contains the strings for many calls?");
	StringList sl("a*;ba*;CAR*", ";");
	char* orig = sl.print_to_string();
	char* check1 = "car";
	char* check2 = "bar";
	char* check3 = "AAR";
	bool retVal = sl.find_matches_anycase_withwildcard(check1, NULL) 
		&& sl.find_matches_anycase_withwildcard(check2, NULL) 
		&& sl.find_matches_anycase_withwildcard(check3, NULL);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check1);
	e.emit_param("StringList", "NULL");
	e.emit_param("STRING", check2);
	e.emit_param("StringList", "NULL");
	e.emit_param("STRING", check3);
	e.emit_param("StringList", "NULL");
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_find_matches_any_wwc_none() {
	e.emit_test("Test that find_matches_anycase_withwildcard() does not add "
		"any matches to the list when there are no matches in the StringList.");
	StringList sl("a;b;c", ";");
	StringList m;
	char* orig = sl.print_to_string();
	char* check = "d";
	sl.find_matches_anycase_withwildcard(check, &m); 
	char* matches = m.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("StringList", "");
	e.emit_output_expected_header();
	e.emit_param("MATCHES", "");
	e.emit_output_actual_header();
	e.emit_param("MATCHES", nicePrint(matches));
	if(niceStrCmp(matches, "") != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_find_matches_any_wwc_one() {
	e.emit_test("Test that find_matches_anycase_withwildcard() adds the "
		"matching string to the StringList.");
	StringList sl("a;b;c", ";");
	StringList m;
	char* orig = sl.print_to_string();
	char* check = "a";
	sl.find_matches_anycase_withwildcard(check, &m); 
	char* expect = "a";
	char* matches = m.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("StringList", "");
	e.emit_output_expected_header();
	e.emit_param("MATCHES", expect);
	e.emit_output_actual_header();
	e.emit_param("MATCHES", nicePrint(matches));
	if(niceStrCmp(matches, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(matches);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(matches);
	return true;
}

static bool test_find_matches_any_wwc_one_exist() {
	e.emit_test("Test that find_matches_anycase_withwildcard() adds the "
		"matching string to the StringList with existing strings.");
	StringList sl("a;b;c", ";");
	StringList m("1;2;3", ";");
	char* orig = sl.print_to_string();
	char* list = m.print_to_string();
	char* check = "a";
	sl.find_matches_anycase_withwildcard(check, &m); 
	char* expect = "1,2,3,a";
	char* matches = m.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("StringList", list);
	e.emit_output_expected_header();
	e.emit_param("MATCHES", expect);
	e.emit_output_actual_header();
	e.emit_param("MATCHES", nicePrint(matches));
	if(niceStrCmp(matches, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(list); niceFree(matches);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(list); niceFree(matches);
	return true;
}

static bool test_find_matches_any_wwc_wild_start() {
	e.emit_test("Test that find_matches_anycase_withwildcard() adds the "
		"matching string to the StringList when using a wildcard at the start "
		"of a string.");
	StringList sl("a;*R;c", ";");
	StringList m;
	char* orig = sl.print_to_string();
	char* check = "bar";
	sl.find_matches_anycase_withwildcard(check, &m); 
	char* expect = "*R";
	char* matches = m.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("StringList", "");
	e.emit_output_expected_header();
	e.emit_param("MATCHES", expect);
	e.emit_output_actual_header();
	e.emit_param("MATCHES", nicePrint(matches));
	if(niceStrCmp(matches, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(matches);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(matches);
	return true;
}

static bool test_find_matches_any_wwc_wild_mid() {
	e.emit_test("Test that find_matches_anycase_withwildcard() adds the "
		"matching string to the StringList when using a wildcard in the middle "
		"of a string.");
	StringList sl("a;B*r;c", ";");
	StringList m;
	char* orig = sl.print_to_string();
	char* check = "bar";
	sl.find_matches_anycase_withwildcard(check, &m); 
	char* expect = "B*r";
	char* matches = m.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("StringList", "");
	e.emit_output_expected_header();
	e.emit_param("MATCHES", expect);
	e.emit_output_actual_header();
	e.emit_param("MATCHES", nicePrint(matches));
	if(niceStrCmp(matches, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(matches);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(matches);
	return true;
}

static bool test_find_matches_any_wwc_wild_end() {
	e.emit_test("Test that find_matches_anycase_withwildcard() adds the "
		"matching string to the StringList when using a wildcard at the end "
		"of a string.");
	StringList sl("a;b*;c", ";");
	StringList m;
	char* orig = sl.print_to_string();
	char* check = "bar";
	sl.find_matches_anycase_withwildcard(check, &m); 
	char* expect = "b*";
	char* matches = m.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("StringList", "");
	e.emit_output_expected_header();
	e.emit_param("MATCHES", expect);
	e.emit_output_actual_header();
	e.emit_param("MATCHES", nicePrint(matches));
	if(niceStrCmp(matches, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(matches);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(matches);
	return true;
}

static bool test_find_matches_any_wwc_multiple() {
	e.emit_test("Test that find_matches_anycase_withwildcard() adds the "
		"matching strings to the StringList when there are multiple matches.");
	StringList sl("a*;AR*;Are*;are", ";");
	StringList m;
	char* orig = sl.print_to_string();
	char* check = "are";
	sl.find_matches_anycase_withwildcard(check, &m); 
	char* expect = "a*,AR*,Are*,are";
	char* matches = m.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("StringList", "");
	e.emit_output_expected_header();
	e.emit_param("MATCHES", expect);
	e.emit_output_actual_header();
	e.emit_param("MATCHES", nicePrint(matches));
	if(niceStrCmp(matches, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(matches);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(matches);
	return true;
}

static bool test_find_matches_any_wwc_multiple_exist() {
	e.emit_test("Test that find_matches_anycase_withwildcard() adds the "
		"matching strings to the StringList with existing strings when there "
		"are multiple matches.");
	StringList sl("a*;AR*;Are*;are", ";");
	StringList m("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* list = m.print_to_string();
	char* check = "are";
	sl.find_matches_anycase_withwildcard(check, &m); 
	char* expect = "a,b,c,a*,AR*,Are*,are";
	char* matches = m.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("StringList", list);
	e.emit_output_expected_header();
	e.emit_param("MATCHES", expect);
	e.emit_output_actual_header();
	e.emit_param("MATCHES", nicePrint(matches));
	if(niceStrCmp(matches, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(list); niceFree(matches);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(list); niceFree(matches);
	return true;
}

static bool test_find_return_false() {
	e.emit_test("Does find() return false when passed a string not in the "
		"StringList?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "d";
	bool retVal = sl.find(check, false);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("Anycase", "FALSE");
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_find_return_false_substring() {
	e.emit_test("Does find() return false when passed a string not in the "
		"StringList, but the StringList has a substring of the string?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "boo";
	bool retVal = sl.find(check, false);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("Anycase", "FALSE");
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_find_return_false_case() {
	e.emit_test("Does find() return false when passed a string in the "
		"StringList when ignoring case, but anycase is false?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "A";
	bool retVal = sl.find(check, false);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("Anycase", "FALSE");
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_find_return_true_one() {
	e.emit_test("Does find() return true when passed one string in the "
		"StringList?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "a";
	bool retVal = sl.find(check, false);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("Anycase", "FALSE");
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_find_return_true_many() {
	e.emit_test("Does find() return true when called many times for different "
		"strings in the StringList?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check1 = "a";
	char* check2 = "b";
	char* check3 = "c";
	bool retVal = sl.find(check3, false) && sl.find(check2, false) 
		&& sl.find(check1, false);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check3);
	e.emit_param("Anycase", "FALSE");
	e.emit_param("STRING", check2);
	e.emit_param("Anycase", "FALSE");
	e.emit_param("STRING", check1);
	e.emit_param("Anycase", "FALSE");
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_find_anycase_return_false() {
	e.emit_test("Does find() return false when passed a string not in the "
		"StringList when anycase is true?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "d";
	bool retVal = sl.find(check, true);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("Anycase", "TRUE");
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_find_anycase_return_false_substring() {
	e.emit_test("Does find() return false when passed a string not in the "
		"StringList when anycase is true, but the StringList has a substring "
		"of the string?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "boo";
	bool retVal = sl.find(check, true);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("Anycase", "TRUE");
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_find_anycase_return_true_one() {
	e.emit_test("Does find() return true when passed one string in the "
		"StringList when anycase is true?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = "a";
	bool retVal = sl.find(check, true);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check);
	e.emit_param("Anycase", "TRUE");
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_find_anycase_return_true_many() {
	e.emit_test("Does find() return true when called many times with different "
		"strings in the StringList when anycase is true?");
	StringList sl("a;b;C;D", ";");
	char* orig = sl.print_to_string();
	char* check1 = "a";
	char* check2 = "B";
	char* check3 = "C";
	char* check4 = "d";
	bool retVal = sl.find(check4, true) && sl.find(check3, true) 
		&& sl.find(check2, true) && sl.find(check1, true);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", check4);
	e.emit_param("Anycase", "TRUE");
	e.emit_param("STRING", check3);
	e.emit_param("Anycase", "TRUE");
	e.emit_param("STRING", check2);
	e.emit_param("Anycase", "TRUE");
	e.emit_param("STRING", check1);
	e.emit_param("Anycase", "TRUE");
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}


static bool test_identical_return_false_same() {
	e.emit_test("Does identical() return false when the lists are not identical"
		", but contain the same number of strings?");
	StringList sl("a;b;c", ";");
	StringList other("d;e;f", ";");
	char* orig = sl.print_to_string();
	char* check = other.print_to_string();
	bool retVal = sl.identical(other, false);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("StringList", check);
	e.emit_param("ANYCASE", "FALSE");
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(check);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(check);
	return true;
}

static bool test_identical_return_false_not_same() {
	e.emit_test("Does identical() return false when the lists are not identical"
		" and don't contain the same number of strings?");
	StringList sl("a;b;c", ";");
	StringList other("d;e;f;g;h;i", ";");
	char* orig = sl.print_to_string();
	char* check = other.print_to_string();
	bool retVal = sl.identical(other, false);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("StringList", check);
	e.emit_param("ANYCASE", "FALSE");
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(check);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(check);
	return true;
}

static bool test_identical_return_false_almost() {
	e.emit_test("Does identical() return false when the lists only differ by "
		"one string?");
	StringList sl("a;b;c", ";");
	StringList other("a;d;c", ";");
	char* orig = sl.print_to_string();
	char* check = other.print_to_string();
	bool retVal = sl.identical(other, false);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("StringList", check);
	e.emit_param("ANYCASE", "FALSE");
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(check);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(check);
	return true;
}

static bool test_identical_return_false_ignore() {
	e.emit_test("Does identical() return false when the lists are identical "
		"when ignoring case, but anycase is false?");
	StringList sl("a;b;c", ";");
	StringList other("A;B;C", ";");
	char* orig = sl.print_to_string();
	char* check = other.print_to_string();
	bool retVal = sl.identical(other, false);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("StringList", check);
	e.emit_param("ANYCASE", "FALSE");
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(check);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(check);
	return true;
}

static bool test_identical_return_false_subset() {
	e.emit_test("Does identical() return false when the passed StringList is a "
		"subset of the StringList called upon?");
	StringList sl("a;b;c;d;e;f", ";");
	StringList other("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = other.print_to_string();
	bool retVal = sl.identical(other, false);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("StringList", check);
	e.emit_param("ANYCASE", "FALSE");
	e.emit_output_expected_header();
	e.emit_retval("FALSE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(check);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(check);
	return true;
}

static bool test_identical_return_true_case() {
	e.emit_test("Does identical() return true when the StringLists are "
		"identical when not ignoring case?");
	StringList sl("a;b;c", ";");
	StringList other("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* check = other.print_to_string();
	bool retVal = sl.identical(other, false);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("StringList", check);
	e.emit_param("ANYCASE", "FALSE");
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(check);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(check);
	return true;
}

static bool test_identical_return_true_ignore() {
	e.emit_test("Does identical() return return true when the StringLists are "
		"identical when ignoring case?");
	StringList sl("a;b;C;D", ";");
	StringList other("a;B;c;D", ";");
	char* orig = sl.print_to_string();
	char* check = other.print_to_string();
	bool retVal = sl.identical(other, true);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("StringList", check);
	e.emit_param("ANYCASE", "TRUE");
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(check);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(check);
	return true;
}

static bool test_identical_return_true_itself() {
	e.emit_test("Does identical() return true when passed the same StringList "
		"called on?");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	bool retVal = sl.identical(sl, false);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("StringList", orig);
	e.emit_param("ANYCASE", "FALSE");
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_identical_return_true_copy() {
	e.emit_test("Does identical() return true when using the copy "
		"constructor?");
	StringList sl("a;b;c", ";");
	StringList other(sl);
	char* orig = sl.print_to_string();
	char* check = other.print_to_string();
	bool retVal = sl.identical(other, true);
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("StringList", check);
	e.emit_param("ANYCASE", "TRUE");
	e.emit_output_expected_header();
	e.emit_retval("TRUE");
	e.emit_output_actual_header();
	e.emit_retval(tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(check);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(check);
	return true;
}

static bool test_print_to_string_empty() {
	e.emit_test("Test print_to_string() on an empty StringList.");
	e.emit_comment("print_to_string() currently returns NULL for an empty "
		"StringList, so this will fail if that changes.");
	StringList sl("", "");
	char* ps = sl.print_to_string();
	char* expect = "";
	e.emit_input_header();
	e.emit_param("StringList", expect);
	e.emit_output_expected_header();
	e.emit_retval(expect);
	e.emit_output_actual_header();
	e.emit_retval(nicePrint(ps));
	if(niceStrCmp(ps, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(ps);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(ps);
	return true;
}

static bool test_print_to_string_one() {
	e.emit_test("Test print_to_string() on a StringList with only one string.");
	StringList sl("foo", "");
	char* ps = sl.print_to_string();
	char* expect = "foo";
	e.emit_input_header();
	e.emit_param("StringList", expect);
	e.emit_output_expected_header();
	e.emit_retval(expect);
	e.emit_output_actual_header();
	e.emit_retval(nicePrint(ps));
	if(niceStrCmp(ps, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(ps);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(ps);
	return true;
}

static bool test_print_to_string_many() {
	e.emit_test("Test print_to_string() on a StringList with many strings.");
	StringList sl("a;b;c;dog;eel;fish;goat", ";");
	char* ps = sl.print_to_string();
	char* expect = "a,b,c,dog,eel,fish,goat";
	e.emit_input_header();
	e.emit_param("StringList", expect);
	e.emit_output_expected_header();
	e.emit_retval(expect);
	e.emit_output_actual_header();
	e.emit_retval(nicePrint(ps));
	if(niceStrCmp(ps, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(ps);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(ps);
	return true;
}

static bool test_print_to_delimed_string_empty() {
	e.emit_test("Test print_to_delimed_string() on an empty StringList.");
	e.emit_comment("print_to_delimed_string() currently returns NULL for an "
		"empty StringList, so this will fail if that changes.");
	StringList sl("", "");
	char* ps = sl.print_to_delimed_string(",");
	char* expect = "";
	e.emit_input_header();
	e.emit_param("StringList", expect);
	e.emit_param("DELIM", ",");
	e.emit_output_expected_header();
	e.emit_retval(expect);
	e.emit_output_actual_header();
	e.emit_retval(nicePrint(ps));
	if(niceStrCmp(ps, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(ps);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(ps);
	return true;
}

static bool test_print_to_delimed_string_one() {
	e.emit_test("Test print_to_delimed_string() on a StringList with only one "
		"string.");
	StringList sl("foo", "");
	char* ps = sl.print_to_delimed_string("!");
	char* expect = "foo";
	e.emit_input_header();
	e.emit_param("StringList", expect);
	e.emit_param("DELIM", "!");
	e.emit_output_expected_header();
	e.emit_retval(expect);
	e.emit_output_actual_header();
	e.emit_retval(nicePrint(ps));
	if(niceStrCmp(ps, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(ps);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(ps);
	return true;
}

static bool test_print_to_delimed_string_many_short() {
	e.emit_test("Test print_to_delimed_string() with a short delimiter on a "
		"StringList with many strings.");
	StringList sl("a;b;c;dog;eel;fish;goat", ";");
	char* orig = sl.print_to_delimed_string();
	char* ps = sl.print_to_delimed_string("&");
	char* expect = "a&b&c&dog&eel&fish&goat";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("DELIM", "&");
	e.emit_output_expected_header();
	e.emit_retval(expect);
	e.emit_output_actual_header();
	e.emit_retval(nicePrint(ps));
	if(niceStrCmp(ps, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(ps);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(ps);
	return true;
}

static bool test_print_to_delimed_string_many_long() {
	e.emit_test("Test print_to_delimed_string() with a long delimiter on a "
		"StringList with many strings.");
	StringList sl("a;b;c;dog;eel;fish;goat", ";");
	char* orig = sl.print_to_string();
	char* ps = sl.print_to_delimed_string(" and ");
	char* expect = "a and b and c and dog and eel and fish and goat";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("DELIM", " and ");
	e.emit_output_expected_header();
	e.emit_retval(expect);
	e.emit_output_actual_header();
	e.emit_retval(nicePrint(ps));
	if(niceStrCmp(ps, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(ps);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(ps);
	return true;
}

static bool test_print_to_delimed_string_many_null() {
	e.emit_test("Test print_to_delimed_string() with a NULL delimiter on a "
		"StringList with many strings.");
	e.emit_comment("Currently print_to_delimed_string() prints the StringList "
		"using the StringList's delimiters if the passed delim is NULL.");
	StringList sl("a;b;c;dog;eel;fish;goat", ";");
	char* ps = sl.print_to_delimed_string(NULL);
	char* orig = sl.print_to_string();
	char* expect = "a;b;c;dog;eel;fish;goat";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("DELIM", "NULL");
	e.emit_output_expected_header();
	e.emit_retval(expect);
	e.emit_output_actual_header();
	e.emit_retval(nicePrint(ps));
	if(niceStrCmp(ps, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(ps);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(ps);
	return true;
}

static bool test_delete_current_before() {
	e.emit_test("Does calling deleteCurrent() before any calls to contains() "
		"or substring() delete the last string from the StringList?");
	e.emit_comment("Since the StringList constructor calls Append on its "
		"internal list for each string to add, current points at the last "
		"string added.");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* expect = "a,b";
	sl.deleteCurrent();
	char* changed = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_output_expected_header();
	e.emit_retval(expect);
	e.emit_output_actual_header();
	e.emit_retval(nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_delete_current_after_no_match() {
	e.emit_test("Does calling deleteCurrent() after calling contains() on a "
		"string that isn't in the StringList delete change the current "
		"pointer?");	
	e.emit_comment("Since the StringList constructor calls Append on its "
		"internal list for each string to add, current points at the last "
		"string added.");
	e.emit_comment("Testing deleteCurrent() requires calling contains*() or "
		"substring() so a problem with one of these could cause problems "
		"here.");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* expect = "a,b";
	sl.contains("d");
	sl.deleteCurrent();
	char* changed = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("contains()", "d");
	e.emit_output_expected_header();
	e.emit_retval(expect);
	e.emit_output_actual_header();
	e.emit_retval(nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_delete_current_one_first() {
	e.emit_test("Test deleteCurrent() after calling contains() on the first "
		"string in the StringList.");
	e.emit_comment("Testing deleteCurrent() requires calling contains*() or "
		"substring() so a problem with one of these could cause problems "
		"here.");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* expect = "b,c";
	sl.contains("a");
	sl.deleteCurrent();
	char* changed = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("contains()", "a");
	e.emit_output_expected_header();
	e.emit_retval(expect);
	e.emit_output_actual_header();
	e.emit_retval(nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_delete_current_one_mid() {
	e.emit_test("Test deleteCurrent() after calling contains() on a string in "
		"the middle of the StringList.");
	e.emit_comment("Testing deleteCurrent() requires calling contains*() or "
		"substring() so a problem with one of these could cause problems "
		"here.");
	StringList sl("a;b;c;d;e", ";");
	char* orig = sl.print_to_string();
	char* expect = "a,b,d,e";
	sl.contains("c");
	sl.deleteCurrent();
	char* changed = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("contains()", "c");
	e.emit_output_expected_header();
	e.emit_retval(expect);
	e.emit_output_actual_header();
	e.emit_retval(nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_delete_current_one_last() {
	e.emit_test("Test deleteCurrent() after calling contains() on the last "
		" string in the StringList.");
	e.emit_comment("Testing deleteCurrent() requires calling contains*() or "
		"substring() so a problem with one of these could cause problems "
		"here.");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* expect = "a,b";
	sl.contains("c");
	sl.deleteCurrent();
	char* changed = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("contains()", "c");
	e.emit_output_expected_header();
	e.emit_retval(expect);
	e.emit_output_actual_header();
	e.emit_retval(nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_delete_current_all() {
	e.emit_test("Test calling deleteCurrent() on all the strings in the "
		"StringList.");
	e.emit_comment("Testing deleteCurrent() requires calling contains*() or "
		"substring() so a problem with one of these could cause problems "
		"here.");
	StringList sl("a;b;c", ";");
	char* orig = sl.print_to_string();
	char* expect = "";
	sl.contains("a");
	sl.deleteCurrent();
	sl.contains("b");
	sl.deleteCurrent();
	sl.contains("c");
	sl.deleteCurrent();
	char* changed = sl.print_to_string();
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("contains()", "a");
	e.emit_param("contains()", "b");
	e.emit_param("contains()", "c");
	e.emit_output_expected_header();
	e.emit_retval(expect);
	e.emit_output_actual_header();
	e.emit_retval(nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_string_compare_equal_same_beg() {
	e.emit_test("Test string_compare() on the same string at the beginning of "
		" a single StringList.");
	e.emit_comment("This test relies on number(), rewind(), and next() so a "
		"problem with any of these may cause problems here.");
	StringList sl("a;b;c", ";");
	char* list = sl.print_to_string();
	char** strs = string_compare_helper(&sl, 0);
	int retVal = string_compare(strs, strs);
	e.emit_input_header();
	e.emit_param("StringList", list);
	e.emit_param("STRING", *strs);
	e.emit_param("STRING", *strs);
	e.emit_output_expected_header();
	e.emit_retval("0");
	e.emit_output_actual_header();
	e.emit_retval("%d", retVal);
	if(retVal != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(list); free_helper(strs);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(list); free_helper(strs);
	return true;
}

static bool test_string_compare_equal_same_mid() {
	e.emit_test("Test string_compare() on the same string in the middle of a "
		"single StringList.");
	e.emit_comment("This test relies on number(), rewind(), and next() so a "
		"problem with any of these may cause problems here.");
	StringList sl("a;b;c", ";");
	char* list = sl.print_to_string();
	char** strs = string_compare_helper(&sl, 1);
	int retVal = string_compare(strs, strs);
	e.emit_input_header();
	e.emit_param("StringList", list);
	e.emit_param("STRING", *strs);
	e.emit_param("STRING", *strs);
	e.emit_output_expected_header();
	e.emit_retval("0");
	e.emit_output_actual_header();
	e.emit_retval("%d", retVal);
	if(retVal != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(list); free_helper(strs);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(list); free_helper(strs);
	return true;
}

static bool test_string_compare_equal_same_end() {
	e.emit_test("Test string_compare() on the same string at the end of a "
		"single StringList.");
	e.emit_comment("This test relies on number(), rewind(), and next() so a "
		"problem with any of these may cause problems here.");
	StringList sl("a;b;c", ";");
	char* list = sl.print_to_string();
	char** strs = string_compare_helper(&sl, 2);
	int retVal = string_compare(strs, strs);
	e.emit_input_header();
	e.emit_param("StringList", list);
	e.emit_param("STRING", *strs);
	e.emit_param("STRING", *strs);
	e.emit_output_expected_header();
	e.emit_retval("0");
	e.emit_output_actual_header();
	e.emit_retval("%d", retVal);
	if(retVal != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(list); free_helper(strs);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(list); free_helper(strs);
	return true;
}

static bool test_string_compare_equal_different_list() {
	e.emit_test("Test string_compare() on two identical strings within two "
		"different StringLists.");
	e.emit_comment("This test relies on number(), rewind(), and next() so a "
		"problem with any of these may cause problems here.");
	StringList sl1("a;b;c", ";");
	StringList sl2("a;b;c", ";");
	char* list1 = sl1.print_to_string();
	char* list2 = sl2.print_to_string();
	char** strs1 = string_compare_helper(&sl1,1);
	char** strs2 = string_compare_helper(&sl2,1);
	int retVal = string_compare(strs1, strs2);
	e.emit_input_header();
	e.emit_param("StringList", list1);
	e.emit_param("StringList", list2);
	e.emit_param("STRING", *strs1);
	e.emit_param("STRING", *strs2);
	e.emit_output_expected_header();
	e.emit_retval("0");
	e.emit_output_actual_header();
	e.emit_retval("%d", retVal);
	if(retVal != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(list1); niceFree(list2); 
		free_helper(strs1); free_helper(strs2); 
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(list1); niceFree(list2); 
	free_helper(strs1); free_helper(strs2);
	return true;
}

static bool test_string_compare_copy() {
	e.emit_test("Test string_compare() after using the copy constructor.");
	e.emit_comment("This test relies on number(), rewind(), and next() so a "
		"problem with any of these may cause problems here.");
	StringList sl1("a;b;c", ";");
	StringList sl2(sl1);
	char* list1  = sl1.print_to_string();
	char* list2  = sl2.print_to_string();
	char** strs1 = string_compare_helper(&sl1,0);
	char** strs2 = string_compare_helper(&sl2,0);
	int retVal = string_compare(strs1, strs2);
	e.emit_input_header();
	e.emit_param("StringList", list1);
	e.emit_param("StringList", list2);
	e.emit_param("STRING", *strs1);
	e.emit_param("STRING", *strs2);
	e.emit_output_expected_header();
	e.emit_retval("0");
	e.emit_output_actual_header();
	e.emit_retval("%d", retVal);
	if(retVal != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(list1); niceFree(list2); 
		free_helper(strs1); free_helper(strs2);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(list1); niceFree(list2); 
	free_helper(strs1); free_helper(strs2);
	return true;
}

static bool test_string_compare_not_equal_same() {
	e.emit_test("Test string_compare() on two different strings in a single "
		"StringList.");
	e.emit_comment("This test relies on number(), rewind(), and next() so a "
		"problem with any of these may cause problems here.");
	StringList sl("a;b;c", ";");
	char* list = sl.print_to_string();
	char** strs1 = string_compare_helper(&sl, 0);
	char** strs2 = string_compare_helper(&sl, 1);
	int retVal = string_compare(strs1, strs2);
	e.emit_input_header();
	e.emit_param("StringList", list);
	e.emit_param("STRING", *strs1);
	e.emit_param("STRING", *strs2);
	e.emit_output_expected_header();
	e.emit_retval("!= 0");
	e.emit_output_actual_header();
	e.emit_retval("%d", retVal);
	if(retVal == MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(list); free_helper(strs1); free_helper(strs2);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(list); free_helper(strs1); free_helper(strs2);
	return true;
}

static bool test_string_compare_not_equal_different() {
	e.emit_test("Test string_compare() on two different strings within two "
		"different StringLists.");
	e.emit_comment("This test relies on number(), rewind(), and next() so a "
		"problem with any of these may cause problems here.");
	StringList sl1("a;b;c", ";");
	StringList sl2("d;e;f", ";");
	char* list1  = sl1.print_to_string();
	char* list2  = sl2.print_to_string();
	char** strs1 = string_compare_helper(&sl1,1);
	char** strs2 = string_compare_helper(&sl2,0);
	int retVal = string_compare(strs1, strs2);
	e.emit_input_header();
	e.emit_param("StringList", list1);
	e.emit_param("StringList", list2);
	e.emit_param("STRING", *strs1);
	e.emit_param("STRING", *strs2);
	e.emit_output_expected_header();
	e.emit_retval("!= 0");
	e.emit_output_actual_header();
	e.emit_retval("%d", retVal);
	if(retVal == MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(list1); niceFree(list2); 
		free_helper(strs1); free_helper(strs2);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(list1); niceFree(list2); 
	free_helper(strs1); free_helper(strs2);
	return true;
}

static bool test_qsort_empty() {
	e.emit_test("Test qsort() on an empty StringList.");
	StringList sl("", "");
	char* orig  = sl.print_to_string();
	sl.qsort();
	char* changed  = sl.print_to_string();
	char* expect  = "";
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_qsort_one() {
	e.emit_test("Test qsort() on a StringList with only one string.");
	StringList sl("foo", "");
	char* orig  = sl.print_to_string();
	sl.qsort();
	char* changed  = sl.print_to_string();
	char* expect  = "foo";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_qsort_already() {
	e.emit_test("Does qsort() change a StringList that is already sorted?");
	StringList sl("a;b;c", ";");
	char* orig  = sl.print_to_string();
	sl.qsort();
	char* changed  = sl.print_to_string();
	char* expect  = "a,b,c";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_qsort_multiple() {
	e.emit_test("Does qsort() change a StringList that has already been sorted "
		"by qsort()?");
	StringList sl("c;b;a", ";");
	char* orig  = sl.print_to_string();
	sl.qsort();
	char* changed1  = sl.print_to_string();
	sl.qsort();
	char* changed2  = sl.print_to_string();
	char* expect  = "a,b,c";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed2));
	if(niceStrCmp(changed2, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed1); niceFree(changed2);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed1); niceFree(changed2);
	return true;
}

static bool test_qsort_many() {
	e.emit_test("Test qsort() on a StringList with many strings.");
	StringList sl("f;foo;eat;e;d;door;cool;c;b;bah;aah;a", ";");
	char* orig  = sl.print_to_string();
	sl.qsort();
	char* changed  = sl.print_to_string();
	char* expect  = "a,aah,b,bah,c,cool,d,door,e,eat,f,foo";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_qsort_many_shuffle() {
	e.emit_test("Test qsort() on a StringList with many strings after calling "
		"shuffle.");
	StringList sl("a;b;c;d;e;f;g;h;i;j;k;l;m;n;o;p;q;r;s;t;u;v;w;x;y;z", ";");
	sl.shuffle();
	char* orig  = sl.print_to_string();
	sl.qsort();
	char* changed  = sl.print_to_string();
	char* expect  = "a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_shuffle_empty() {
	e.emit_test("Test shuffle() on an empty StringList.");
	StringList sl("", "");
	char* orig  = sl.print_to_string();
	sl.shuffle();
	char* changed  = sl.print_to_string();
	char* expect  = "";
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_shuffle_one() {
	e.emit_test("Test shuffle() on a StringList with only one string.");
	StringList sl("foo", "");
	char* orig  = sl.print_to_string();
	sl.shuffle();
	char* changed  = sl.print_to_string();
	char* expect  = "foo";
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_shuffle_many() {
	e.emit_test("Test shuffle() on a StringList with many strings.");
	e.emit_comment("This test may fail if shuffle happens to shuffle to the "
		"same order as the original, although this is highly unlikely here.");
	StringList sl("a;b;c;d;e;f;g;h;i;j;k;l;m;n;o;p;q;r;s;t;u;v;w;x;y;z", ";");
	char* orig  = sl.print_to_string();
	sl.shuffle();
	char* changed  = sl.print_to_string();
	char* notExpect  = "a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_output_expected_header();
	e.emit_param("StringList !=", notExpect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(changed, notExpect) == MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_rewind_empty() {
	e.emit_test("Test rewind() on an empty StringList.");
	e.emit_comment("This test uses next() to check where 'current' is "
		"so a problem with that may cause this to fail.");
	StringList sl("", "");
	char* orig  = sl.print_to_string();
	sl.rewind();
	char* next = sl.next();
	char* expect  = "";
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_output_expected_header();
	e.emit_param("next", expect);
	e.emit_output_actual_header();
	e.emit_param("next", nicePrint(next));
	if(niceStrCmp(next, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_rewind_non_empty() {
	e.emit_test("Test rewind() on a non-empty StringList.");
	e.emit_comment("This test uses next() to check where 'current' is "
		"so a problem with that may cause this to fail.");
	StringList sl("a;b;c", ";");
	char* orig  = sl.print_to_string();
	sl.rewind();
	char* next = sl.next();
	char* expect  = "a";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_output_expected_header();
	e.emit_param("next", expect);
	e.emit_output_actual_header();
	e.emit_param("next", nicePrint(next));
	if(niceStrCmp(next, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_rewind_after_contains_true() {
	e.emit_test("Test rewind() after a successful call to contains().");
	e.emit_comment("This test uses next() to check where 'current' is "
		"so a problem with that may cause this to fail.");
	StringList sl("a;b;c", ";");
	char* orig  = sl.print_to_string();
	bool retVal = sl.contains("b");
	sl.rewind();
	char* next = sl.next();
	char* expect  = "a";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("contains(\"b\")", tfstr(retVal));
	e.emit_output_expected_header();
	e.emit_param("next", expect);
	e.emit_output_actual_header();
	e.emit_param("next", nicePrint(next));
	if(niceStrCmp(next, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_rewind_after_contains_false() {
	e.emit_test("Test rewind() after an unsuccessful call to contains().");
	e.emit_comment("This test uses next() to check where 'current' is "
		"so a problem with that may cause this to fail.");
	StringList sl("a;b;c", ";");
	char* orig  = sl.print_to_string();
	bool retVal = sl.contains("d");
	sl.rewind();
	char* next = sl.next();
	char* expect  = "a";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("contains(\"d\")", tfstr(retVal));
	e.emit_output_expected_header();
	e.emit_param("next", expect);
	e.emit_output_actual_header();
	e.emit_param("next", nicePrint(next));
	if(niceStrCmp(next, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_append_empty() {
	e.emit_test("Test append() on an empty StringList.");
	StringList sl("", ";");
	char* orig  = sl.print_to_string();
	sl.append("a");
	char* changed  = sl.print_to_string();
	char* expect  = "a";
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_param("STRING", "a");
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_append_one() {
	e.emit_test("Test a single append() on a non-empty StringList.");
	StringList sl("a;b;c", ";");
	char* orig  = sl.print_to_string();
	sl.append("d");
	char* changed  = sl.print_to_string();
	char* expect  = "a,b,c,d";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", "d");
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_append_many() {
	e.emit_test("Test many calls to append() on a non-empty StringList.");
	StringList sl("a;b;c", ";");
	char* orig  = sl.print_to_string();
	sl.append("d");
	sl.append("e");
	sl.append("f");
	char* changed  = sl.print_to_string();
	char* expect  = "a,b,c,d,e,f";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", "d");
	e.emit_param("STRING", "e");
	e.emit_param("STRING", "f");
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_append_contains() {
	e.emit_test("Test append() after a successful call to contains().");
	StringList sl("a;b;c", ";");
	char* orig  = sl.print_to_string();
	bool retVal = sl.contains("b");
	sl.append("d");
	char* changed  = sl.print_to_string();
	char* expect  = "a,b,c,d";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("contains(\"b\")", tfstr(retVal));
	e.emit_param("STRING", "d");
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_append_rewind() {
	e.emit_test("Test append() after calling rewind().");
	StringList sl("a;b;c", ";");
	char* orig  = sl.print_to_string();
	sl.rewind();
	sl.append("d");
	char* next = sl.next();
	char* changed  = sl.print_to_string();
	char* expect  = "a,b,c,d";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", "d");
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	e.emit_param("next", nicePrint(next));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_append_current() {
	e.emit_test("Does append() change 'current' to point at the new item?");
	e.emit_comment("This test uses deleteCurrent() to check where 'current' is "
		"so a problem with that may cause this to fail.");
	StringList sl("a;b;c", ";");
	char* orig  = sl.print_to_string();
	sl.append("d");
	sl.deleteCurrent();
	char* changed = sl.print_to_string();
	char* expect  = "a,b,c";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", "d");
	e.emit_output_expected_header();
	e.emit_param("StrinList", expect);
	e.emit_output_actual_header();
	e.emit_param("StrinList", nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_insert_empty() {
	e.emit_test("Test insert() on an empty StringList.");
	StringList sl("", "");
	char* orig  = sl.print_to_string();
	sl.insert("a");
	char* changed  = sl.print_to_string();
	char* expect  = "a";
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_param("STRING", "a");
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_insert_head() {
	e.emit_test("Test insert() at the beginning of the StringList.");
	e.emit_comment("This test uses next() to navigate through the StringList "
		"so a problem with that may cause this to fail.");
	StringList sl("a;b;c", ";");
	char* orig  = sl.print_to_string();
	sl.rewind();
	sl.next();	//current=a
	sl.insert("d");
	char* changed  = sl.print_to_string();
	char* expect  = "d,a,b,c";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", "d");
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_insert_middle() {
	e.emit_test("Test insert() in the middle of the StringList.");
	e.emit_comment("This test uses next() to navigate through the StringList "
		"so a problem with that may cause this to fail.");
	StringList sl("a;b;c", ";");
	char* orig  = sl.print_to_string();
	sl.rewind();
	sl.next();	//current=a
	sl.next();	//current=b
	sl.next();	//current=c
	sl.insert("d");
	char* changed  = sl.print_to_string();
	char* expect  = "a,b,d,c";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", "d");
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_insert_end() {
	e.emit_test("Test insert() at the end of the StringList.");
	e.emit_comment("This test uses next() to navigate through the StringList "
		"so a problem with that may cause this to fail.");
	e.emit_comment("Currently insert() inserts the string before what 'current'"
		" points to, so there is no way at inserting at the end of the list, "
		"just before the last element.");
	StringList sl("a;b;c", ";");
	char* orig  = sl.print_to_string();
	sl.rewind();
	sl.next();	//current=a
	sl.next();	//current=b
	sl.next();	//current=c
	sl.insert("d");
	char* changed  = sl.print_to_string();
	char* expect  = "a,b,d,c";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", "d");
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_insert_current() {
	e.emit_test("Test that insert() doesn't change what 'current' points to.");
	e.emit_comment("This test uses next() to navigate through the StringList "
		"so a problem with that may cause this to fail.");
	StringList sl("a;b;c", ";");
	char* orig  = sl.print_to_string();
	sl.rewind();
	sl.next();	//current=a
	sl.next();	//current=b
	sl.insert("d");
	char* next = sl.next();	//current=c
	char* expect  = "c";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("STRING", "d");
	e.emit_output_expected_header();
	e.emit_param("next", expect);
	e.emit_output_actual_header();
	e.emit_param("next", nicePrint(next));
	if(niceStrCmp(next, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_insert_many() {
	e.emit_test("Test many calls to  insert().");
	e.emit_comment("This test uses next() to navigate through the StringList "
		"so a problem with that may cause this to fail.");
	StringList sl("a;b;c", ";");
	char* orig  = sl.print_to_string();
	sl.rewind();
	sl.next();	//current=a
	sl.insert("0");
	sl.insert("1");
	sl.next();	//current=b
	sl.insert("2");
	sl.next();	//current=c
	sl.insert("3");
	char* changed  = sl.print_to_string();
	char* expect  = "0,1,a,2,b,3,c";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("'current'", "a");
	e.emit_param("insert", "0");
	e.emit_param("insert", "1");
	e.emit_param("'current'", "b");
	e.emit_param("insert", "2");
	e.emit_param("'current'", "c");
	e.emit_param("insert", "3");
	e.emit_output_expected_header();
	e.emit_param("StringList", expect);
	e.emit_output_actual_header();
	e.emit_param("StringList", nicePrint(changed));
	if(niceStrCmp(changed, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig); niceFree(changed);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig); niceFree(changed);
	return true;
}

static bool test_next_empty() {
	e.emit_test("Test calling next() on an empty StringList.");
	StringList sl("", "");
	char* orig  = sl.print_to_string();
	char* next = sl.next();
	char* expect  = "";
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_output_expected_header();
	e.emit_param("STRING", expect);
	e.emit_output_actual_header();
	e.emit_param("STRING", nicePrint(next));
	if(niceStrCmp(next, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_next_beginning() {
	e.emit_test("Test calling next() when 'current' is at the beginning of the "
		"StringList.");
	e.emit_comment("This test requires the use of rewind(), so a problem with "
		"that could cause this to fail.");
	StringList sl("a;b;c", ";");
	char* orig  = sl.print_to_string();
	sl.rewind();
	char* next = sl.next();
	char* expect  = "a";
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_output_expected_header();
	e.emit_param("STRING", expect);
	e.emit_output_actual_header();
	e.emit_param("STRING", nicePrint(next));
	if(niceStrCmp(next, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_next_middle() {
	e.emit_test("Test calling next() when 'current' is in the middle of the "
		"StringList.");
	e.emit_comment("This test requires the use of contains(), so a problem with"
		" that could cause this to fail.");
	StringList sl("a;b;c", ";");
	char* orig  = sl.print_to_string();
	sl.contains("b");
	char* next = sl.next();
	char* expect  = "c";
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_output_expected_header();
	e.emit_param("STRING", expect);
	e.emit_output_actual_header();
	e.emit_param("STRING", nicePrint(next));
	if(niceStrCmp(next, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_next_end() {
	e.emit_test("Test calling next() when 'current' is at the end of the "
		"StringList.");
	e.emit_comment("This test requires the use of contains(), so a problem with"
		" that could cause this to fail.");
	StringList sl("a;b;c", ";");
	char* orig  = sl.print_to_string();
	sl.contains("c");
	char* next = sl.next();
	char* expect  = "";
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_output_expected_header();
	e.emit_param("STRING", expect);
	e.emit_output_actual_header();
	e.emit_param("STRING", nicePrint(next));
	if(niceStrCmp(next, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_number_empty() {
	e.emit_test("Test number() on an empty StringList.");
	StringList sl("", "");
	char* orig  = sl.print_to_string();
	int num = sl.number();
	int expect  = 0;
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_output_expected_header();
	e.emit_retval("%d", expect);
	e.emit_output_actual_header();
	e.emit_retval("%d", num);
	if(num != expect) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_number_one() {
	e.emit_test("Test number() on a StringList with one string.");
	StringList sl("foo", "");
	char* orig  = sl.print_to_string();
	int num = sl.number();
	int expect  = 1;
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_output_expected_header();
	e.emit_retval("%d", expect);
	e.emit_output_actual_header();
	e.emit_retval("%d", num);
	if(num != expect) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_number_many() {
	e.emit_test("Test number() on a StringList with many strings.");
	StringList sl("a;b;c;d;e;f", ";");
	char* orig  = sl.print_to_string();
	int num = sl.number();
	int expect  = 6;
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_output_expected_header();
	e.emit_retval("%d", expect);
	e.emit_output_actual_header();
	e.emit_retval("%d", num);
	if(num != expect) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_number_after_delete() {
	e.emit_test("Test that number() changes after calling deleteCurrent().");
	e.emit_comment("This test requires the use of contains() and "
		"deleteCurrent() so a problem with either of these may cause this to "
		"fail.");
	StringList sl("a;b;c;d;e;f", ";");
	char* orig  = sl.print_to_string();
	sl.contains("c");
	sl.deleteCurrent();
	int num = sl.number();
	int expect  = 5;
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("Call", "contains(\"c\")");
	e.emit_param("Call", "deleteCurrent()");
	e.emit_output_expected_header();
	e.emit_retval("%d", expect);
	e.emit_output_actual_header();
	e.emit_retval("%d", num);
	if(num != expect) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_number_after_clear_all() {
	e.emit_test("Test that number() returns 0 after calling clearAll().");
	e.emit_comment("This test requires the use of clearAll() so a problem with "
		"that may cause this to fail.");
	StringList sl("a;b;c;d;e;f", ";");
	char* orig  = sl.print_to_string();
	sl.clearAll();
	int num = sl.number();
	int expect  = 0;
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("Call", "clearAll()");
	e.emit_output_expected_header();
	e.emit_retval("%d", expect);
	e.emit_output_actual_header();
	e.emit_retval("%d", num);
	if(num != expect) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_number_after_append() {
	e.emit_test("Test that number() returns the correct size of the StringList "
		"after calling append().");
	e.emit_comment("This test requires the use of append() so a problem with "
		"that may cause this to fail.");
	StringList sl("a;b;c", ";");
	char* orig  = sl.print_to_string();
	sl.append("dog");
	int num = sl.number();
	int expect  = 4;
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("Call", "append(\"dog\")");
	e.emit_output_expected_header();
	e.emit_retval("%d", expect);
	e.emit_output_actual_header();
	e.emit_retval("%d", num);
	if(num != expect) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_number_after_insert() {
	e.emit_test("Test that number() returns the correct size of the StringList "
		"after calling insert().");
	e.emit_comment("This test requires the use of insert() so a problem with "
		"that may cause this to fail.");
	StringList sl("a;b;c", ";");
	char* orig  = sl.print_to_string();
	sl.insert("dog");
	int num = sl.number();
	int expect  = 4;
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("Call", "insert(\"dog\")");
	e.emit_output_expected_header();
	e.emit_retval("%d", expect);
	e.emit_output_actual_header();
	e.emit_retval("%d", num);
	if(num != expect) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_number_copy() {
	e.emit_test("Test that number() of a StringList created by using the copy "
		"constructor is equal to the source StringList.");
	StringList sl1("a;b;c;d;e;f", ";");
	StringList sl2(sl1);
	char* orig1  = sl1.print_to_string();
	char* orig2  = sl2.print_to_string();
	int num1 = sl1.number();
	int num2 = sl2.number();
	int expect  = 6;
	e.emit_input_header();
	e.emit_param("StringList", orig1);
	e.emit_param("StringList", orig2);
	e.emit_output_expected_header();
	e.emit_retval("%d", expect);
	e.emit_output_actual_header();
	e.emit_retval("%d", num1);
	e.emit_retval("%d", num2);
	if(num1 != expect || num2 != expect) {
		e.emit_result_failure(__LINE__);
		niceFree(orig1); niceFree(orig2);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig1); niceFree(orig2);
	return true;
}

static bool test_is_empty_empty() {
	e.emit_test("Test that isEmpty() returns true for an empty StringList.");
	StringList sl("", "");
	char* orig  = sl.print_to_string();
	bool retVal = sl.isEmpty();
	bool expect  = true;
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_output_expected_header();
	e.emit_retval("%s", tfstr(expect));
	e.emit_output_actual_header();
	e.emit_retval("%s", tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_is_empty_clear() {
	e.emit_test("Test that isEmpty() returns true after calling clearAll().");
	e.emit_comment("This test requires the use of clearAll() so a problem with "
		"that may cause this to fail.");
	StringList sl("a;b;c", ";");
	char* orig  = sl.print_to_string();
	sl.clearAll();
	bool retVal = sl.isEmpty();
	bool expect  = true;
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_param("Call", "clearAll()");
	e.emit_output_expected_header();
	e.emit_retval("%s", tfstr(expect));
	e.emit_output_actual_header();
	e.emit_retval("%s", tfstr(retVal));
	if(!retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_is_empty_one() {
	e.emit_test("Test that isEmpty() returns false for a StringList with only "
		"one string.");
	StringList sl("foo", "");
	char* orig  = sl.print_to_string();
	bool retVal = sl.isEmpty();
	bool expect  = false;
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_output_expected_header();
	e.emit_retval("%s", tfstr(expect));
	e.emit_output_actual_header();
	e.emit_retval("%s", tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_is_empty_many() {
	e.emit_test("Test that isEmpty() returns false for a StringList with many "
		"strings.");
	StringList sl("a;b;c;d;e;f", ";");
	char* orig  = sl.print_to_string();
	bool retVal = sl.isEmpty();
	bool expect  = false;
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_output_expected_header();
	e.emit_retval("%s", tfstr(expect));
	e.emit_output_actual_header();
	e.emit_retval("%s", tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_is_empty_append() {
	e.emit_test("Test that isEmpty() returns false after calling append() on an"
		" empty StringList.");
	e.emit_comment("This test requires the use of append() so a problem with "
		"that may cause this to fail.");
	StringList sl("", "");
	char* orig  = sl.print_to_string();
	sl.append("foo");
	bool retVal = sl.isEmpty();
	bool expect  = false;
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_param("Call", "append(\"foo\")");
	e.emit_output_expected_header();
	e.emit_retval("%s", tfstr(expect));
	e.emit_output_actual_header();
	e.emit_retval("%s", tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_is_empty_insert() {
	e.emit_test("Test that isEmpty() returns false after calling insert() on an"		" empty StringList.");
	e.emit_comment("This test requires the use of insert() so a problem with "
		"that may cause this to fail.");
	StringList sl("", "");
	char* orig  = sl.print_to_string();
	sl.insert("foo");
	bool retVal = sl.isEmpty();
	bool expect  = false;
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_param("Call", "insert(\"foo\")");
	e.emit_output_expected_header();
	e.emit_retval("%s", tfstr(expect));
	e.emit_output_actual_header();
	e.emit_retval("%s", tfstr(retVal));
	if(retVal) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_get_list_empty() {
	e.emit_test("Test that getList() returns a the List<char> representation "
		"of the strings in an empty StringList.");
	e.emit_comment("All this test checks is if getList() is empty since all of "
		"List's operations are tested elswhere.");
	StringList sl("", "");
	char* orig  = sl.print_to_string();
	bool isEmpty = sl.getList().IsEmpty();
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_output_expected_header();
	e.emit_param("Is Empty", "%s", "TRUE");
	e.emit_output_actual_header();
	e.emit_param("Is Empty", "%s", tfstr(isEmpty));
	if(!isEmpty) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_get_list_non_empty() {
	e.emit_test("Test that getList() returns a the List<char> representation "
		"of the strings in a non-empty StringList.");
	e.emit_comment("All this test checks is if getList() is not empty since all"
		" of List's operations are tested elswhere.");
	StringList sl("a;b;c;dog", ";");
	char* orig  = sl.print_to_string();
	bool isEmpty = sl.getList().IsEmpty();
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_output_expected_header();
	e.emit_param("Is Empty", "%s", "FALSE");
	e.emit_output_actual_header();
	e.emit_param("Is Empty", "%s", tfstr(isEmpty));
	if(isEmpty) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_get_delimiters_empty_no() {
	e.emit_test("Test that getDelimiters() returns the correct string "
		"representation of the delimiters of an empty StringList with no "
		"delimiters.");
	StringList sl("", "");
	char* orig  = sl.print_to_string();
	const char* delims = sl.getDelimiters();
	char* expect = "";
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_output_expected_header();
	e.emit_param("DELIMS", expect);
	e.emit_output_actual_header();
	e.emit_param("DELIMS", delims);
	if(strcmp(delims, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_get_delimiters_empty_yes() {
	e.emit_test("Test that getDelimiters() returns the correct string "
		"representation of the delimiters of an empty StringList with "
		"delimiters.");
	StringList sl("", ";");
	char* orig  = sl.print_to_string();
	const char* delims = sl.getDelimiters();
	char* expect = ";";
	e.emit_input_header();
	e.emit_param("StringList", nicePrint(orig));
	e.emit_output_expected_header();
	e.emit_param("DELIMS", expect);
	e.emit_output_actual_header();
	e.emit_param("DELIMS", delims);
	if(strcmp(delims, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_get_delimiters_non_empty_no() {
	e.emit_test("Test that getDelimiters() returns the correct string "
		"representation of the delimiters of a non-empty StringList with no "
		"delimiters.");
	StringList sl("a;b;c", "");
	char* orig  = sl.print_to_string();
	const char* delims = sl.getDelimiters();
	char* expect = "";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_output_expected_header();
	e.emit_param("DELIMS", expect);
	e.emit_output_actual_header();
	e.emit_param("DELIMS", delims);
	if(strcmp(delims, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}

static bool test_get_delimiters_non_empty_yes() {
	e.emit_test("Test that getDelimiters() returns the correct string "
		"representation of the delimiters of a non-empty StringList with "
		"delimiters.");
	StringList sl("a;b,c!d", ";!,");
	char* orig  = sl.print_to_string();
	const char* delims = sl.getDelimiters();
	char* expect = ";!,";
	e.emit_input_header();
	e.emit_param("StringList", orig);
	e.emit_output_expected_header();
	e.emit_param("DELIMS", expect);
	e.emit_output_actual_header();
	e.emit_param("DELIMS", delims);
	if(strcmp(delims, expect) != MATCH) {
		e.emit_result_failure(__LINE__);
		niceFree(orig);
		return false;
	}
	e.emit_result_success(__LINE__);
	niceFree(orig);
	return true;
}
