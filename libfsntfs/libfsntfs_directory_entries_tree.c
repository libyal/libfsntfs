/*
 * Directory entries tree functions
 *
 * Copyright (C) 2010-2023, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libfsntfs_definitions.h"
#include "libfsntfs_directory_entries_tree.h"
#include "libfsntfs_directory_entry.h"
#include "libfsntfs_file_name_values.h"
#include "libfsntfs_index.h"
#include "libfsntfs_index_value.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_mft_attribute.h"
#include "libfsntfs_mft_entry.h"
#include "libfsntfs_name.h"
#include "libfsntfs_standard_information_values.h"
#include "libfsntfs_unused.h"

/* Creates a directory entries tree
 * Make sure the value directory_entries_tree is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_directory_entries_tree_initialize(
     libfsntfs_directory_entries_tree_t **directory_entries_tree,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_directory_entries_tree_initialize";

	if( directory_entries_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries tree.",
		 function );

		return( -1 );
	}
	if( *directory_entries_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory entries tree value already set.",
		 function );

		return( -1 );
	}
	*directory_entries_tree = memory_allocate_structure(
	                           libfsntfs_directory_entries_tree_t );

	if( *directory_entries_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create directory entries tree.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *directory_entries_tree,
	     0,
	     sizeof( libfsntfs_directory_entries_tree_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear directory entries tree.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *directory_entries_tree != NULL )
	{
		memory_free(
		 *directory_entries_tree );

		*directory_entries_tree = NULL;
	}
	return( -1 );
}

/* Frees a directory entries tree
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_directory_entries_tree_free(
     libfsntfs_directory_entries_tree_t **directory_entries_tree,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_directory_entries_tree_free";
	int result            = 1;

	if( directory_entries_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries tree.",
		 function );

		return( -1 );
	}
	if( *directory_entries_tree != NULL )
	{
		if( ( *directory_entries_tree )->i30_index != NULL )
		{
			if( libfsntfs_index_free(
			     &( ( *directory_entries_tree )->i30_index ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free $I30 index.",
				 function );

				result = -1;
			}
		}
		if( ( *directory_entries_tree )->short_names_tree != NULL )
		{
			if( libcdata_btree_free(
			     &( ( *directory_entries_tree )->short_names_tree ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_directory_entry_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free short names tree.",
				 function );

				result = -1;
			}
		}
		if( ( *directory_entries_tree )->entries_list != NULL )
		{
			if( libfdata_list_free(
			     &( ( *directory_entries_tree )->entries_list ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free entries list.",
				 function );

				result = -1;
			}
		}
		if( ( *directory_entries_tree )->entries_cache != NULL )
		{
			if( libfcache_cache_free(
			     &( ( *directory_entries_tree )->entries_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free entries cache.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *directory_entries_tree );

		*directory_entries_tree = NULL;
	}
	return( result );
}

/* Inserts an index value into the directory entries tree
 * Returns 1 if a directory entry was inserted, 0 if not or -1 on error
 */
int libfsntfs_directory_entries_tree_insert_index_value(
     libfsntfs_directory_entries_tree_t *directory_entries_tree,
     int index_value_entry,
     libfsntfs_index_value_t *index_value,
     uint32_t index_value_flags,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *upper_node                      = NULL;
	libfsntfs_directory_entry_t *directory_entry          = NULL;
	libfsntfs_directory_entry_t *existing_directory_entry = NULL;
	libfsntfs_file_name_values_t *file_name_values        = NULL;
	static char *function                                 = "libfsntfs_directory_entries_tree_insert_index_value";
	uint8_t name_space                                    = 0;
	int element_file_index                                = 0;
	int element_index                                     = 0;
	int result                                            = 0;
	int value_index                                       = 0;

	if( directory_entries_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries tree.",
		 function );

		return( -1 );
	}
	if( index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index value.",
		 function );

		return( -1 );
	}
	if( libfsntfs_file_name_values_initialize(
	     &file_name_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file name values.",
		 function );

		goto on_error;
	}
	if( libfsntfs_file_name_values_read_data(
	     file_name_values,
	     index_value->key_data,
	     (size_t) index_value->key_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file name values.",
		 function );

		goto on_error;
	}
	if( file_name_values->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file name values - missing name.",
		 function );

		goto on_error;
	}
	/* Ignore the file name with the . as its name
	 */
	if( ( file_name_values->name_size == 2 )
	 && ( file_name_values->name[ 0 ] == 0x2e )
	 && ( file_name_values->name[ 1 ] == 0x00 ) )
	{
		if( libfsntfs_file_name_values_free(
		     &file_name_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file name values.",
			 function );

			goto on_error;
		}
		return( 0 );
	}
	name_space = file_name_values->name_space;

	if( name_space == LIBFSNTFS_FILE_NAME_SPACE_DOS )
	{
		if( libfsntfs_directory_entry_initialize(
		     &directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create directory entry.",
			 function );

			goto on_error;
		}
		directory_entry->file_reference         = index_value->file_reference;
		directory_entry->short_file_name_values = file_name_values;

		file_name_values = NULL;

		result = libcdata_btree_insert_value(
		          directory_entries_tree->short_names_tree,
		          &value_index,
		          (intptr_t *) directory_entry,
		          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libfsntfs_directory_entry_compare_by_file_reference,
		          &upper_node,
		          (intptr_t **) &existing_directory_entry,
		          error ) ;

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to insert directory entry into tree.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			if( existing_directory_entry == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing existing directory entry.",
				 function );

				goto on_error;
			}
			if( name_space == LIBFSNTFS_FILE_NAME_SPACE_DOS )
			{
				if( existing_directory_entry->short_file_name_values != NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
					 "%s: short file name of existing directory entry already set.",
					 function );

					goto on_error;
				}
				existing_directory_entry->short_file_name_values = directory_entry->short_file_name_values;
				directory_entry->short_file_name_values          = NULL;
			}
			else
			{
				if( existing_directory_entry->file_name_values != NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
					 "%s: file name of existing directory entry already set.",
					 function );

					goto on_error;
				}
				existing_directory_entry->file_name_values = directory_entry->file_name_values;
				directory_entry->file_name_values          = NULL;
			}
			if( libfsntfs_directory_entry_free(
			     &directory_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free directory entry.",
				 function );

				goto on_error;
			}
		}
	}
	if( name_space != LIBFSNTFS_FILE_NAME_SPACE_DOS )
	{
		/* The element file index value contains the index value entry + 1
		 */
		element_file_index = index_value_entry + 1;

		if( libfdata_list_append_element(
		     directory_entries_tree->entries_list,
		     &element_index,
		     element_file_index,
		     index_value->offset,
		     (size64_t) index_value->size,
		     index_value_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append index value to entries list.",
			 function );

			goto on_error;
		}
	}
	if( file_name_values != NULL )
	{
		if( libfsntfs_file_name_values_free(
		     &file_name_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file name values.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( directory_entry != NULL )
	{
		libfsntfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	if( file_name_values != NULL )
	{
		libfsntfs_file_name_values_free(
		 &file_name_values,
		 NULL );
	}
	return( -1 );
}

/* Reads the directory entries tree from an index node
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_directory_entries_tree_read_from_index_node(
     libfsntfs_directory_entries_tree_t *directory_entries_tree,
     libbfio_handle_t *file_io_handle,
     libfsntfs_index_node_t *index_node,
     int recursion_depth,
     libcerror_error_t **error )
{
	libfcache_cache_t *sub_node_cache    = NULL;
	libfsntfs_index_node_t *sub_node     = NULL;
	libfsntfs_index_value_t *index_value = NULL;
	static char *function                = "libfsntfs_directory_entries_tree_read_from_index_node";
	off64_t index_entry_offset           = 0;
	uint32_t index_value_flags           = 0;
	int index_value_entry                = 0;
	int is_allocated                     = 0;
	int number_of_index_values           = 0;
	int result                           = 0;

	if( directory_entries_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries tree.",
		 function );

		return( -1 );
	}
	if( directory_entries_tree->i30_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries tree - missing $I30 index.",
		 function );

		return( -1 );
	}
	if( directory_entries_tree->i30_index->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries tree - invalid $I30 index - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( recursion_depth < 0 )
	 || ( recursion_depth > LIBFSNTFS_MAXIMUM_RECURSION_DEPTH ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid recursion depth value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfsntfs_index_node_get_number_of_values(
	     index_node,
	     &number_of_index_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values from index node.",
		 function );

		goto on_error;
	}
	/* Use a local cache to prevent cache invalidation of index node
	 * when reading sub nodes.
	 */
	if( libfcache_cache_initialize(
	     &sub_node_cache,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub node cache.",
		 function );

		goto on_error;
	}
	for( index_value_entry = 0;
	     index_value_entry < number_of_index_values;
	     index_value_entry++ )
	{
		if( libfsntfs_index_node_get_value_by_index(
		     index_node,
		     index_value_entry,
		     &index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %d from index node.",
			 function,
			 index_value_entry );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: index value: %03d file reference: %" PRIu64 "-%" PRIu64 "\n",
			 function,
			 index_value_entry,
			 index_value->file_reference & 0xffffffffffffUL,
			 index_value->file_reference >> 48 );

			libcnotify_printf(
			 "\n" );
		}
#endif
		if( ( index_value->flags & LIBFSNTFS_INDEX_VALUE_FLAG_IS_BRANCH_NODE ) != 0 )
		{
			if( index_value->sub_node_vcn > (uint64_t) INT_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: node index value: %d sub node VCN value out of bounds.",
				 function,
				 index_value_entry );

				goto on_error;
			}
			is_allocated = libfsntfs_index_sub_node_is_allocated(
			                directory_entries_tree->i30_index,
			                (int) index_value->sub_node_vcn,
			                error );

			if( is_allocated == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine if sub node with VCN: %d is allocated.",
				 function,
				 (int) index_value->sub_node_vcn );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: index value: %03d sub node VCN: %" PRIu64 " (%s).\n",
				 function,
				 index_value_entry,
				 index_value->sub_node_vcn,
				 is_allocated ? "allocated" : "unallocated" );

				libcnotify_printf(
				 "\n" );
			}
#endif
			if( is_allocated == 0 )
			{
				continue;
			}
			index_entry_offset = (off64_t) ( index_value->sub_node_vcn * directory_entries_tree->i30_index->io_handle->cluster_block_size );

			if( libfsntfs_index_get_sub_node(
			     directory_entries_tree->i30_index,
			     file_io_handle,
			     sub_node_cache,
			     index_entry_offset,
			     (int) index_value->sub_node_vcn,
			     &sub_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub node with VCN: %d at offset: %" PRIi64 " (0x%08" PRIx64 ").",
				 function,
				 (int) index_value->sub_node_vcn,
				 index_entry_offset,
				 index_entry_offset );

				goto on_error;
			}
			if( libfsntfs_directory_entries_tree_read_from_index_node(
			     directory_entries_tree,
			     file_io_handle,
			     sub_node,
			     recursion_depth + 1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read directory entries tree from index entry with VCN: %d at offset: %" PRIi64 " (0x%08" PRIx64 ").",
				 function,
				 (int) index_value->sub_node_vcn,
				 index_entry_offset,
				 index_entry_offset );

				goto on_error;
			}
		}
		if( ( index_value->flags & LIBFSNTFS_INDEX_VALUE_FLAG_IS_LAST ) != 0 )
		{
			break;
		}
		index_value_flags = 0;

		if( index_node == directory_entries_tree->i30_index->root_node )
		{
			index_value_flags = LIBFSNTFS_INDEX_VALUE_LIST_FLAG_STORED_IN_ROOT;
		}
		/* Add the index values containing data in a depth first manner since
		 * this will preserve the sorted by file name order of the directory entries
		 */
		result = libfsntfs_directory_entries_tree_insert_index_value(
		          directory_entries_tree,
		          index_value_entry,
		          index_value,
		          index_value_flags,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to insert index value into directory entries tree.",
			 function );

			goto on_error;
		}
	}
	if( libfcache_cache_free(
	     &sub_node_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free sub node cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( sub_node_cache != NULL )
	{
		libfcache_cache_free(
		 &sub_node_cache,
		 NULL );
	}
	return( -1 );
}

/* Reads the directory entries tree from the $I30 index
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_directory_entries_tree_read_from_i30_index(
     libfsntfs_directory_entries_tree_t *directory_entries_tree,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsntfs_mft_entry_t *mft_entry,
     uint8_t flags,
     libcerror_error_t **error )
{
	libfsntfs_mft_attribute_t *mft_attribute                             = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	static char *function                                                = "libfsntfs_directory_entries_tree_read_from_i30_index";
	uint32_t attribute_type                                              = 0;
	uint32_t collation_type                                              = 0;
	int result                                                           = 0;

	if( directory_entries_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries tree.",
		 function );

		return( -1 );
	}
	if( directory_entries_tree->i30_index != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory entries tree - $I30 index value already set.",
		 function );

		return( -1 );
	}
	if( directory_entries_tree->short_names_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory entries tree - short names tree value already set.",
		 function );

		return( -1 );
	}
	if( ( flags & LIBFSNTFS_FILE_ENTRY_FLAGS_MFT_ONLY ) != 0 )
	{
		return( 1 );
	}
	if( libfsntfs_index_initialize(
	     &( directory_entries_tree->i30_index ),
	     io_handle,
	     (uint8_t *) "$I30",
	     5,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create $I30 index.",
		 function );

		goto on_error;
	}
	result = libfsntfs_index_read(
	          directory_entries_tree->i30_index,
	          file_io_handle,
	          mft_entry,
	          flags,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read $I30 index.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfsntfs_index_get_attribute_type(
		     directory_entries_tree->i30_index,
		     &attribute_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute type from index.",
			 function );

			goto on_error;
		}
		if( attribute_type != LIBFSNTFS_ATTRIBUTE_TYPE_FILE_NAME )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported index attribute type.",
			 function );

			goto on_error;
		}
		if( libfsntfs_index_get_collation_type(
		     directory_entries_tree->i30_index,
		     &collation_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve collation type from index.",
			 function );

			goto on_error;
		}
		if( collation_type != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported index collation type.",
			 function );

			goto on_error;
		}
		if( libfdata_list_initialize(
		     &( directory_entries_tree->entries_list ),
		     (intptr_t *) directory_entries_tree,
		     NULL,
		     NULL,
		     (int (*)(intptr_t *, intptr_t *, libfdata_list_element_t *, libfdata_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsntfs_directory_entries_tree_read_element_data,
		     NULL,
		     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create entries list.",
			 function );

			goto on_error;
		}
		if( libfcache_cache_initialize(
		     &( directory_entries_tree->entries_cache ),
		     1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create entries cache.",
			 function );

			goto on_error;
		}
		if( libcdata_btree_initialize(
		     &( directory_entries_tree->short_names_tree ),
		     LIBFSNTFS_INDEX_TREE_MAXIMUM_NUMBER_OF_SUB_NODES,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create short names B-tree.",
			 function );

			goto on_error;
		}
		if( libfsntfs_directory_entries_tree_read_from_index_node(
		     directory_entries_tree,
		     file_io_handle,
		     directory_entries_tree->i30_index->root_node,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read directory entries tree from root node.",
			 function );

			goto on_error;
		}
		directory_entries_tree->use_case_folding = 0;

		result = libfsntfs_mft_entry_get_standard_information_attribute(
		          mft_entry,
		          &mft_attribute,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve $STANDARD_INFORMATION attribute from MFT entry.",
			 function );

			goto on_error;
		}
		else if( result != 0 )
		{
			if( libfsntfs_standard_information_values_initialize(
			     &standard_information_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create standard information values.",
				 function );

				goto on_error;
			}
			if( libfsntfs_standard_information_values_read_from_mft_attribute(
			     standard_information_values,
			     mft_attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read standard information values from MFT attribute.",
				 function );

				goto on_error;
			}
			if( standard_information_values->is_case_sensitive == 0 )
			{
				directory_entries_tree->use_case_folding = 1;
			}
			if( libfsntfs_standard_information_values_free(
			     &standard_information_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free standard information values.",
				 function );

				goto on_error;
			}
		}
	}
	return( 1 );

on_error:
	if( standard_information_values != NULL )
	{
		libfsntfs_standard_information_values_free(
		 &standard_information_values,
		 NULL );
	}
	if( directory_entries_tree->short_names_tree != NULL )
	{
		libcdata_btree_free(
		 &( directory_entries_tree->short_names_tree ),
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_directory_entry_free,
		 NULL );
	}
	if( directory_entries_tree->entries_cache != NULL )
	{
		libfcache_cache_free(
		 &( directory_entries_tree->entries_cache ),
		 NULL );
	}
	if( directory_entries_tree->entries_list != NULL )
	{
		libfdata_list_free(
		 &( directory_entries_tree->entries_list ),
		 NULL );
	}
	if( directory_entries_tree->i30_index != NULL )
	{
		libfsntfs_index_free(
		 &( directory_entries_tree->i30_index ),
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of entries
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_directory_entries_tree_get_number_of_entries(
     libfsntfs_directory_entries_tree_t *directory_entries_tree,
     int *number_of_entries,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_directory_entries_tree_get_number_of_entries";

	if( directory_entries_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries tree.",
		 function );

		return( -1 );
	}
	if( directory_entries_tree->entries_list == NULL )
	{
		if( number_of_entries == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid number of entries.",
			 function );

			return( -1 );
		}
		*number_of_entries = 0;
	}
	else
	{
		if( libfdata_list_get_number_of_elements(
		     directory_entries_tree->entries_list,
		     number_of_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of elements from entries list.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves a specific entry
 * This function creates a new directory entry
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_directory_entries_tree_get_entry_by_index(
     libfsntfs_directory_entries_tree_t *directory_entries_tree,
     libbfio_handle_t *file_io_handle,
     int entry_index,
     libfsntfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfsntfs_directory_entry_t *safe_directory_entry = NULL;
	static char *function                             = "libfsntfs_directory_entries_tree_get_entry_by_index";

	if( directory_entries_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries tree.",
		 function );

		return( -1 );
	}
	if( libfdata_list_get_element_value_by_index(
	     directory_entries_tree->entries_list,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) directory_entries_tree->entries_cache,
	     entry_index,
	     (intptr_t **) &safe_directory_entry,
	     LIBFDATA_READ_FLAG_IGNORE_CACHE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element: %d from entries list.",
		 function,
		 entry_index );

		return( -1 );
	}
	if( libfsntfs_directory_entry_clone(
	     directory_entry,
	     safe_directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create directory entry.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the directory entry from an index node for an UTF-8 encoded name
 * This function creates a new directory entry
 * Returns 1 if successful, 0 if no such directory entry or -1 on error
 */
int libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf8_name(
     libfsntfs_directory_entries_tree_t *directory_entries_tree,
     libbfio_handle_t *file_io_handle,
     libfsntfs_index_node_t *index_node,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsntfs_directory_entry_t **directory_entry,
     int recursion_depth,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *upper_node                      = NULL;
	libfsntfs_directory_entry_t *existing_directory_entry = NULL;
	libfsntfs_directory_entry_t *safe_directory_entry     = NULL;
	libfsntfs_file_name_values_t *file_name_values        = NULL;
	libfsntfs_index_node_t *sub_node                      = NULL;
	libfsntfs_index_value_t *index_value                  = NULL;
	static char *function                                 = "libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf8_name";
	off64_t index_entry_offset                            = 0;
	int compare_result                                    = 0;
	int index_value_entry                                 = 0;
	int is_allocated                                      = 0;
	int number_of_index_values                            = 0;
	int result                                            = 0;

	if( directory_entries_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries tree.",
		 function );

		return( -1 );
	}
	if( directory_entries_tree->i30_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries tree - missing $I30 index.",
		 function );

		return( -1 );
	}
	if( directory_entries_tree->i30_index->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries tree - invalid $I30 index - missing IO handle.",
		 function );

		return( -1 );
	}
	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( ( recursion_depth < 0 )
	 || ( recursion_depth > LIBFSNTFS_MAXIMUM_RECURSION_DEPTH ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid recursion depth value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfsntfs_index_node_get_number_of_values(
	     index_node,
	     &number_of_index_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values from index node.",
		 function );

		goto on_error;
	}
	for( index_value_entry = 0;
	     index_value_entry < number_of_index_values;
	     index_value_entry++ )
	{
		if( libfsntfs_index_node_get_value_by_index(
		     index_node,
		     index_value_entry,
		     &index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %d from index node.",
			 function,
			 index_value_entry );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: index value: %03d file reference: %" PRIu64 "-%" PRIu64 "\n",
			 function,
			 index_value_entry,
			 index_value->file_reference & 0xffffffffffffUL,
			 index_value->file_reference >> 48 );

			libcnotify_printf(
			 "\n" );
		}
#endif
		if( ( index_value->flags & LIBFSNTFS_INDEX_VALUE_FLAG_IS_BRANCH_NODE ) != 0 )
		{
			if( index_value->sub_node_vcn > (uint64_t) INT_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: node index value: %d sub node VCN value out of bounds.",
				 function,
				 index_value_entry );

				goto on_error;
			}
			is_allocated = libfsntfs_index_sub_node_is_allocated(
			                directory_entries_tree->i30_index,
			                (int) index_value->sub_node_vcn,
			                error );

			if( is_allocated == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine if sub node with VCN: %d is allocated.",
				 function,
				 (int) index_value->sub_node_vcn );

				goto on_error;
			}
			else if( is_allocated == 0 )
			{
				continue;
			}
		}
		if( ( index_value->flags & LIBFSNTFS_INDEX_VALUE_FLAG_IS_LAST ) != 0 )
		{
			break;
		}
		if( libfsntfs_file_name_values_initialize(
		     &file_name_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file name values.",
			 function );

			goto on_error;
		}
		if( libfsntfs_file_name_values_read_data(
		     file_name_values,
		     index_value->key_data,
		     (size_t) index_value->key_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read file name values.",
			 function );

			goto on_error;
		}
		compare_result = libfsntfs_name_compare_with_utf8_string(
		                  file_name_values->name,
		                  file_name_values->name_size,
		                  utf8_string,
		                  utf8_string_length,
		                  directory_entries_tree->use_case_folding,
		                  error );

		if( compare_result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare file name values with UTF-8 string.",
			 function );

			goto on_error;
		}
		if( compare_result != LIBUNA_COMPARE_EQUAL )
		{
			if( libfsntfs_file_name_values_free(
			     &file_name_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free file name values.",
				 function );

				goto on_error;
			}
		}
		if( compare_result == LIBUNA_COMPARE_LESS )
		{
			if( ( index_value->flags & LIBFSNTFS_INDEX_VALUE_FLAG_IS_BRANCH_NODE ) != 0 )
			{
				break;
			}
		}
		else if( compare_result == LIBUNA_COMPARE_EQUAL )
		{
			break;
		}
	}
	if( compare_result == LIBUNA_COMPARE_EQUAL )
	{
		if( libfsntfs_directory_entry_initialize(
		     &safe_directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create directory entry.",
			 function );

			goto on_error;
		}
		safe_directory_entry->file_reference   = index_value->file_reference;
		safe_directory_entry->file_name_values = file_name_values;

		file_name_values = NULL;

		if( safe_directory_entry->file_name_values->name_space == LIBFSNTFS_FILE_NAME_SPACE_WINDOWS )
		{
			result = libcdata_btree_get_value_by_value(
			          directory_entries_tree->short_names_tree,
			          (intptr_t *) directory_entry,
			          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libfsntfs_directory_entry_compare_by_file_reference,
			          &upper_node,
			          (intptr_t **) &existing_directory_entry,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve directory entry from tree.",
				 function );

				goto on_error;
			}
			else if( result != 0 )
			{
/* TODO add debug compare of long and short names */
				if( existing_directory_entry == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing existing directory entry.",
					 function );

					goto on_error;
				}
				if( libfsntfs_file_name_values_clone(
				     &( safe_directory_entry->short_file_name_values ),
				     existing_directory_entry->short_file_name_values,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to clone short file name values.",
					 function );

					goto on_error;
				}
			}
		}
		*directory_entry = safe_directory_entry;

		result = 1;
	}
	else if( ( index_value->flags & LIBFSNTFS_INDEX_VALUE_FLAG_IS_BRANCH_NODE ) != 0 )
	{
		index_entry_offset = (off64_t) ( index_value->sub_node_vcn * directory_entries_tree->i30_index->io_handle->cluster_block_size );

		if( libfsntfs_index_get_sub_node(
		     directory_entries_tree->i30_index,
		     file_io_handle,
		     directory_entries_tree->i30_index->index_node_cache,
		     index_entry_offset,
		     (int) index_value->sub_node_vcn,
		     &sub_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub node with VCN: %d at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 (int) index_value->sub_node_vcn,
			 index_entry_offset,
			 index_entry_offset );

			goto on_error;
		}
		result = libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf8_name(
		          directory_entries_tree,
		          file_io_handle,
		          sub_node,
		          utf8_string,
		          utf8_string_length,
		          directory_entry,
		          recursion_depth + 1,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to retrieve directory entry by UTF-8 string from index entry with VCN: %d at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 (int) index_value->sub_node_vcn,
			 index_entry_offset,
			 index_entry_offset );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( safe_directory_entry != NULL )
	{
		libfsntfs_directory_entry_free(
		 &safe_directory_entry,
		 NULL );
	}
	if( file_name_values != NULL )
	{
		libfsntfs_file_name_values_free(
		 &file_name_values,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the directory entry for an UTF-8 encoded name
 * This function creates a new directory entry
 * Returns 1 if successful, 0 if no such directory entry or -1 on error
 */
int libfsntfs_directory_entries_tree_get_entry_by_utf8_name(
     libfsntfs_directory_entries_tree_t *directory_entries_tree,
     libbfio_handle_t *file_io_handle,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsntfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_directory_entries_tree_get_entry_by_utf8_name";
	int result            = 0;

	if( directory_entries_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries tree.",
		 function );

		return( -1 );
	}
	if( directory_entries_tree->i30_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries tree - missing $I30 index.",
		 function );

		return( -1 );
	}
	result = libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf8_name(
	          directory_entries_tree,
	          file_io_handle,
	          directory_entries_tree->i30_index->root_node,
	          utf8_string,
	          utf8_string_length,
	          directory_entry,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry by UTF-8 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the directory entry from an index node for an UTF-16 encoded name
 * This function creates a new directory entry
 * Returns 1 if successful, 0 if no such directory entry or -1 on error
 */
int libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf16_name(
     libfsntfs_directory_entries_tree_t *directory_entries_tree,
     libbfio_handle_t *file_io_handle,
     libfsntfs_index_node_t *index_node,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsntfs_directory_entry_t **directory_entry,
     int recursion_depth,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *upper_node                      = NULL;
	libfsntfs_directory_entry_t *existing_directory_entry = NULL;
	libfsntfs_directory_entry_t *safe_directory_entry     = NULL;
	libfsntfs_file_name_values_t *file_name_values        = NULL;
	libfsntfs_index_node_t *sub_node                      = NULL;
	libfsntfs_index_value_t *index_value                  = NULL;
	static char *function                                 = "libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf16_name";
	off64_t index_entry_offset                            = 0;
	int compare_result                                    = 0;
	int index_value_entry                                 = 0;
	int is_allocated                                      = 0;
	int number_of_index_values                            = 0;
	int result                                            = 0;

	if( directory_entries_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries tree.",
		 function );

		return( -1 );
	}
	if( directory_entries_tree->i30_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries tree - missing $I30 index.",
		 function );

		return( -1 );
	}
	if( directory_entries_tree->i30_index->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries tree - invalid $I30 index - missing IO handle.",
		 function );

		return( -1 );
	}
	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( ( recursion_depth < 0 )
	 || ( recursion_depth > LIBFSNTFS_MAXIMUM_RECURSION_DEPTH ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid recursion depth value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfsntfs_index_node_get_number_of_values(
	     index_node,
	     &number_of_index_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values from index node.",
		 function );

		goto on_error;
	}
	for( index_value_entry = 0;
	     index_value_entry < number_of_index_values;
	     index_value_entry++ )
	{
		if( libfsntfs_index_node_get_value_by_index(
		     index_node,
		     index_value_entry,
		     &index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %d from index node.",
			 function,
			 index_value_entry );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: index value: %03d file reference: %" PRIu64 "-%" PRIu64 "\n",
			 function,
			 index_value_entry,
			 index_value->file_reference & 0xffffffffffffUL,
			 index_value->file_reference >> 48 );

			libcnotify_printf(
			 "\n" );
		}
#endif
		if( ( index_value->flags & LIBFSNTFS_INDEX_VALUE_FLAG_IS_BRANCH_NODE ) != 0 )
		{
			if( index_value->sub_node_vcn > (uint64_t) INT_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: node index value: %d sub node VCN value out of bounds.",
				 function,
				 index_value_entry );

				goto on_error;
			}
			is_allocated = libfsntfs_index_sub_node_is_allocated(
			                directory_entries_tree->i30_index,
			                (int) index_value->sub_node_vcn,
			                error );

			if( is_allocated == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine if sub node with VCN: %d is allocated.",
				 function,
				 (int) index_value->sub_node_vcn );

				goto on_error;
			}
			else if( is_allocated == 0 )
			{
				continue;
			}
		}
		if( ( index_value->flags & LIBFSNTFS_INDEX_VALUE_FLAG_IS_LAST ) != 0 )
		{
			break;
		}
		if( libfsntfs_file_name_values_initialize(
		     &file_name_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file name values.",
			 function );

			goto on_error;
		}
		if( libfsntfs_file_name_values_read_data(
		     file_name_values,
		     index_value->key_data,
		     (size_t) index_value->key_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read file name values.",
			 function );

			goto on_error;
		}
		compare_result = libfsntfs_name_compare_with_utf16_string(
		                  file_name_values->name,
		                  file_name_values->name_size,
		                  utf16_string,
		                  utf16_string_length,
		                  directory_entries_tree->use_case_folding,
		                  error );

		if( compare_result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare file name values with UTF-16 string.",
			 function );

			goto on_error;
		}
		if( compare_result != LIBUNA_COMPARE_EQUAL )
		{
			if( libfsntfs_file_name_values_free(
			     &file_name_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free file name values.",
				 function );

				goto on_error;
			}
		}
		if( compare_result == LIBUNA_COMPARE_LESS )
		{
			if( ( index_value->flags & LIBFSNTFS_INDEX_VALUE_FLAG_IS_BRANCH_NODE ) != 0 )
			{
				break;
			}
		}
		else if( compare_result == LIBUNA_COMPARE_EQUAL )
		{
			break;
		}
	}
	if( compare_result == LIBUNA_COMPARE_EQUAL )
	{
		if( libfsntfs_directory_entry_initialize(
		     &safe_directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create directory entry.",
			 function );

			goto on_error;
		}
		safe_directory_entry->file_reference   = index_value->file_reference;
		safe_directory_entry->file_name_values = file_name_values;

		file_name_values = NULL;

		if( safe_directory_entry->file_name_values->name_space == LIBFSNTFS_FILE_NAME_SPACE_WINDOWS )
		{
			result = libcdata_btree_get_value_by_value(
			          directory_entries_tree->short_names_tree,
			          (intptr_t *) directory_entry,
			          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libfsntfs_directory_entry_compare_by_file_reference,
			          &upper_node,
			          (intptr_t **) &existing_directory_entry,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve directory entry from tree.",
				 function );

				goto on_error;
			}
			else if( result != 0 )
			{
/* TODO add debug compare of long and short names */
				if( existing_directory_entry == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing existing directory entry.",
					 function );

					goto on_error;
				}
				if( libfsntfs_file_name_values_clone(
				     &( safe_directory_entry->short_file_name_values ),
				     existing_directory_entry->short_file_name_values,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to clone short file name values.",
					 function );

					goto on_error;
				}
			}
		}
		*directory_entry = safe_directory_entry;

		result = 1;
	}
	else if( ( index_value->flags & LIBFSNTFS_INDEX_VALUE_FLAG_IS_BRANCH_NODE ) != 0 )
	{
		index_entry_offset = (off64_t) ( index_value->sub_node_vcn * directory_entries_tree->i30_index->io_handle->cluster_block_size );

		if( libfsntfs_index_get_sub_node(
		     directory_entries_tree->i30_index,
		     file_io_handle,
		     directory_entries_tree->i30_index->index_node_cache,
		     index_entry_offset,
		     (int) index_value->sub_node_vcn,
		     &sub_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub node with VCN: %d at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 (int) index_value->sub_node_vcn,
			 index_entry_offset,
			 index_entry_offset );

			goto on_error;
		}
		result = libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf16_name(
		          directory_entries_tree,
		          file_io_handle,
		          sub_node,
		          utf16_string,
		          utf16_string_length,
		          directory_entry,
		          recursion_depth + 1,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to retrieve directory entry by UTF-16 string from index entry with VCN: %d at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 (int) index_value->sub_node_vcn,
			 index_entry_offset,
			 index_entry_offset );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( safe_directory_entry != NULL )
	{
		libfsntfs_directory_entry_free(
		 &safe_directory_entry,
		 NULL );
	}
	if( file_name_values != NULL )
	{
		libfsntfs_file_name_values_free(
		 &file_name_values,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the directory entry for an UTF-16 encoded name
 * This function creates a new directory entry
 * Returns 1 if successful, 0 if no such directory entry or -1 on error
 */
int libfsntfs_directory_entries_tree_get_entry_by_utf16_name(
     libfsntfs_directory_entries_tree_t *directory_entries_tree,
     libbfio_handle_t *file_io_handle,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsntfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_directory_entries_tree_get_entry_by_utf16_name";
	int result            = 0;

	if( directory_entries_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries tree.",
		 function );

		return( -1 );
	}
	if( directory_entries_tree->i30_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries tree - missing $I30 index.",
		 function );

		return( -1 );
	}
	result = libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf16_name(
	          directory_entries_tree,
	          file_io_handle,
	          directory_entries_tree->i30_index->root_node,
	          utf16_string,
	          utf16_string_length,
	          directory_entry,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry by UTF-16 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Reads a directory entry
 * Callback function for the entries list
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_directory_entries_tree_read_element_data(
     libfsntfs_directory_entries_tree_t *directory_entries_tree,
     libbfio_handle_t *file_io_handle,
     libfdata_list_element_t *element,
     libfdata_cache_t *cache,
     int element_file_index,
     off64_t index_value_offset,
     size64_t element_size LIBFSNTFS_ATTRIBUTE_UNUSED,
     uint32_t index_value_flags,
     uint8_t read_flags LIBFSNTFS_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *upper_node                      = NULL;
	libfsntfs_directory_entry_t *directory_entry          = NULL;
	libfsntfs_directory_entry_t *existing_directory_entry = NULL;
	libfsntfs_index_node_t *sub_node                      = NULL;
	libfsntfs_index_value_t *index_value                  = NULL;
	static char *function                                 = "libfsntfs_directory_entries_tree_read_element_data";
	off64_t index_entry_offset                            = 0;
	off64_t sub_node_vcn                                  = 0;
	int index_value_entry                                 = 0;
	int is_allocated                                      = 0;
	int result                                            = 0;

	LIBFSNTFS_UNREFERENCED_PARAMETER( element_size )
	LIBFSNTFS_UNREFERENCED_PARAMETER( read_flags )

	if( directory_entries_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries tree.",
		 function );

		return( -1 );
	}
	if( directory_entries_tree->i30_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries tree - missing $I30 index.",
		 function );

		return( -1 );
	}
	if( directory_entries_tree->i30_index->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries tree - invalid $I30 index - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libfdata_list_element_get_element_index(
	     element,
	     &index_value_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element index.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading index value: %03d at offset: %" PRIi64 " (0x%08" PRIx64 ").\n",
		 function,
		 index_value_entry,
		 index_value_offset,
		 index_value_offset );
	}
#endif
	/* The element file index value contains the index value entry + 1
	 */
	element_file_index -= 1;

	if( ( index_value_flags & LIBFSNTFS_INDEX_VALUE_LIST_FLAG_STORED_IN_ROOT ) != 0 )
	{
		if( libfsntfs_index_node_get_value_by_index(
		     directory_entries_tree->i30_index->root_node,
		     element_file_index,
		     &index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve index value: %d from root node.",
			 function,
			 element_file_index );

			goto on_error;
		}
	}
	else
	{
		sub_node_vcn       = index_value_offset / directory_entries_tree->i30_index->io_handle->cluster_block_size;
		index_entry_offset = (off64_t) ( sub_node_vcn * directory_entries_tree->i30_index->io_handle->cluster_block_size );

		is_allocated = libfsntfs_index_sub_node_is_allocated(
		                directory_entries_tree->i30_index,
		                (int) sub_node_vcn,
		                error );

		if( is_allocated == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if sub node with VCN: %d is allocated.",
			 function,
			 (int) sub_node_vcn );

			goto on_error;
		}
		else if( is_allocated == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported unallocated sub node with VCN: %d.",
			 function,
			 (int) sub_node_vcn );

			goto on_error;
		}
		if( libfsntfs_index_get_sub_node(
		     directory_entries_tree->i30_index,
		     file_io_handle,
		     directory_entries_tree->i30_index->index_node_cache,
		     index_entry_offset,
		     (int) sub_node_vcn,
		     &sub_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub node with VCN: %d at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 (int) sub_node_vcn,
			 index_entry_offset,
			 index_entry_offset );

			goto on_error;
		}
		if( libfsntfs_index_node_get_value_by_index(
		     sub_node,
		     element_file_index,
		     &index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve index value: %d from sub node.",
			 function,
			 element_file_index );

			goto on_error;
		}
	}
	if( libfsntfs_directory_entry_initialize(
	     &directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create directory entry.",
		 function );

		goto on_error;
	}
	if( libfsntfs_file_name_values_initialize(
	     &( directory_entry->file_name_values ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file name values.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: index value: %03d file reference: %" PRIu64 "-%" PRIu64 "\n",
		 function,
		 index_value_entry,
		 index_value->file_reference & 0xffffffffffffUL,
		 index_value->file_reference >> 48 );

		libcnotify_printf(
		 "\n" );
	}
#endif
	directory_entry->file_reference = index_value->file_reference;

	if( libfsntfs_file_name_values_read_data(
	     directory_entry->file_name_values,
	     index_value->key_data,
	     (size_t) index_value->key_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file name values.",
		 function );

		goto on_error;
	}
	if( directory_entry->file_name_values->name_space == LIBFSNTFS_FILE_NAME_SPACE_WINDOWS )
	{
		result = libcdata_btree_get_value_by_value(
		          directory_entries_tree->short_names_tree,
		          (intptr_t *) directory_entry,
		          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libfsntfs_directory_entry_compare_by_file_reference,
		          &upper_node,
		          (intptr_t **) &existing_directory_entry,
		          error ) ;

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entry from tree.",
			 function );

			goto on_error;
		}
		else if( result != 0 )
		{
/* TODO add debug compare of long and short names */
			if( existing_directory_entry == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing existing directory entry.",
				 function );

				goto on_error;
			}
			if( libfsntfs_file_name_values_clone(
			     &( directory_entry->short_file_name_values ),
			     existing_directory_entry->short_file_name_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to clone short file name values.",
				 function );

				goto on_error;
			}
		}
	}
	if( libfdata_list_element_set_element_value(
	     element,
	     (intptr_t *) file_io_handle,
	     cache,
	     (intptr_t *) directory_entry,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_directory_entry_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set index value as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( directory_entry != NULL )
	{
		libfsntfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	return( -1 );
}

