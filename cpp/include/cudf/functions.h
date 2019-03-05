#pragma once

/** 
 * @brief  Start a NVTX range with predefined color.
 *
 * This function is useful only for profiling with nvvp or Nsight Systems. It
 * demarcates the begining of a user-defined range with a specified name and
 * color that will show up in the timeline view of nvvp/Nsight Systems. Can be
 * nested within other ranges.
 * 
 * @param[in] name The name of the NVTX range
 * @param[in] color The predefined gdf_color enum to use to color this range
 * 
 * @returns   
 */
gdf_error gdf_nvtx_range_push(char const * const name, gdf_color color );

/** 
 * @brief  Start a NVTX range with a custom ARGB color code.
 *
 * This function is useful only for profiling with nvvp or Nsight Systems. It
 * demarcates the begining of a user-defined range with a specified name and
 * color that will show up in the timeline view of nvvp/Nsight Systems. Can be
 * nested within other ranges.
 * 
 * @param[in] name The name of the NVTX range
 * @param[in] color The ARGB hex color code to use to color this range (e.g., 0xFF00FF00)
 * 
 * @returns   
 */
gdf_error gdf_nvtx_range_push_hex(char const * const name, unsigned int color );

/** 
 * @brief Ends the inner-most NVTX range.
 *
 * This function is useful only for profiling with nvvp or Nsight Systems. It
 * will demarcate the end of the inner-most range, i.e., the most recent call to
 * gdf_nvtx_range_push.
 * 
 * @returns   
 */
gdf_error gdf_nvtx_range_pop();

/** 
 * @brief  Counts the number of valid bits in the mask that corresponds to
 * the specified number of rows.
 * 
 * @param[in] masks Array of gdf_valid_types with enough bits to represent
 * num_rows number of rows
 * @param[in] num_rows The number of rows represented in the bit-validity mask.
 * @param[out] count The number of valid rows in the mask
 * 
 * @returns  GDF_SUCCESS upon successful completion. 
 */
gdf_error gdf_count_nonzero_mask(gdf_valid_type const *masks,
                                 gdf_size_type num_rows, gdf_size_type *count);


/* column operations */

/** 
 * @brief Return the size of the gdf_column data type.
 *
 * @returns gdf_size_type Size of the gdf_column data type.
 */
gdf_size_type gdf_column_sizeof();

/** 
 * @brief Create a GDF column given data and validity bitmask pointers, size, and
 *        datatype
 *
 * @param[out] column The output column.
 * @param[in] data Pointer to data.
 * @param[in] valid Pointer to validity bitmask for the data.
 * @param[in] size Number of rows in the column.
 * @param[in] dtype Data type of the column.
 * 
 * @returns gdf_error returns GDF_SUCCESS upon successful creation.
 */
gdf_error gdf_column_view(gdf_column *column, void *data, gdf_valid_type *valid,
                          gdf_size_type size, gdf_dtype dtype);

/** 
 * @brief Create a GDF column given data and validity bitmask pointers, size, and
 *        datatype, and count of null (non-valid) elements
 *
 * @param[out] column The output column.
 * @param[in] data Pointer to data.
 * @param[in] valid Pointer to validity bitmask for the data.
 * @param[in] size Number of rows in the column.
 * @param[in] dtype Data type of the column.
 * @param[in] null_count The number of non-valid elements in the validity bitmask.
 * @param[in] extra_info see gdf_dtype_extra_info. Extra data for column description.
 * 
 * @returns gdf_error returns GDF_SUCCESS upon successful creation.
 */
gdf_error gdf_column_view_augmented(gdf_column *column, void *data, gdf_valid_type *valid,
                          gdf_size_type size, gdf_dtype dtype, gdf_size_type null_count,
                          gdf_dtype_extra_info extra_info);

/** 
 * @brief Free the CUDA device memory of a gdf_column
 *
 * @param[in,out] column Data and validity bitmask pointers of this column will be freed
 * 
 * @returns gdf_error GDF_SUCCESS or GDF_ERROR if there is an error freeing the data
 */
gdf_error gdf_column_free(gdf_column *column);

/** ---------------------------------------------------------------------------*
 * @brief Concatenates multiple gdf_columns into a single, contiguous column,
 * including the validity bitmasks.
 * 
 * Note that input columns with nullptr validity masks are treated as if all
 * elements are valid.
 *
 * @param[out] output_column A column whose buffers are already allocated that
 *             will contain the concatenation of the input columns data and
 *             validity bitmasks
 * @param[in] columns_to_concat[] The columns to concatenate
 * @param[in] num_columns The number of columns to concatenate
 * 
 * @return gdf_error GDF_SUCCESS upon completion; GDF_DATASET_EMPTY if any data
 *         pointer is NULL, GDF_COLUMN_SIZE_MISMATCH if the output column size
 *         != the total size of the input columns; GDF_DTYPE_MISMATCH if the
 *         input columns have different datatypes.
 * ---------------------------------------------------------------------------**/
gdf_error gdf_column_concat(gdf_column *output, gdf_column *columns_to_concat[], int num_columns);


/* context operations */

/**
 * @brief  Constructor for the gdf_context struct
 *
 * @param[out] gdf_context being constructed
 * @param[in] Indicates if the input data is sorted. 0 = No, 1 = yes
 * @param[in] the method to be used for the operation (e.g., sort vs hash)
 * @param[in] for COUNT: DISTINCT = 1, else = 0
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_context_view(gdf_context *context, int flag_sorted, gdf_method flag_method,
                           int flag_distinct, int flag_sort_result, int flag_sort_inplace);


/* error handling */

/**
 * @brief  Converts a gdf_error error code into a string
 *
 * @param[in] gdf_error
 *
 * @returns  name of the error
 */
const char * gdf_error_get_name(gdf_error errcode);

/**
 * @brief  returns the last error from a runtime call.
 *
 * @returns  last error from a runtime call.
 */
int gdf_cuda_last_error();

/**
 * @brief  returns the description string for an error code.
 *
 * @param[in] cuda error code
 *
 * @returns  description string for an error code.
 */
const char * gdf_cuda_error_string(int cuda_error);

/**
 * @brief  returns the string representation of an error code enum name.
 *
 * @param[in] cuda error code
 *
 * @returns  string representation of an error code enum name.
 */
const char * gdf_cuda_error_name(int cuda_error);


/* ipc */

/**
 * @brief  Opens a parser from a pyarrow RecordBatch schema
 *
 * @param[in] Pointer to a byte array containing the pyarrow RecordBatch schema
 * @param[in] Size of the byte array
 *
 * @returns Pointer to a parsing struct gdf_ipc_parser_type
 */
gdf_ipc_parser_type* gdf_ipc_parser_open(const uint8_t *schema, size_t length);

/**
 * @brief  Opens a pyarrow RecordBatch bytearray
 *
 * @param[in] Pointer to a parsing struct gdf_ipc_parser_type
 * @param[in] Pointer to a pyarrow RecordBatch bytearray
 * @param[in] Size of the byte array
 *
 * @returns
 */
void gdf_ipc_parser_open_recordbatches(gdf_ipc_parser_type *handle,
                                       const uint8_t *recordbatches,
                                       size_t length);

/**
 * @brief  Closes a parser from a pyarrow RecordBatch schema
 *
 * @param[in] Pointer to a parsing struct gdf_ipc_parser_type
 *
 * @returns void
 */
void gdf_ipc_parser_close(gdf_ipc_parser_type *handle);

/**
 * @brief  Checks for a failure in the parser
 *
 * @param[in] Pointer to a parsing struct gdf_ipc_parser_type
 *
 * @returns 1 if error
 */
int gdf_ipc_parser_failed(gdf_ipc_parser_type *handle);

/**
 * @brief  returns parsed data as json
 *
 * @param[in] Pointer to a parsing struct gdf_ipc_parser_type
 *
 * @returns char* of parsed data as json
 */
const char* gdf_ipc_parser_to_json(gdf_ipc_parser_type *handle);

/**
 * @brief  Gets error from gdf_ipc_parser_type
 *
 * @param[in] Pointer to a parsing struct gdf_ipc_parser_type
 *
 * @returns Error message as char*
 */
const char* gdf_ipc_parser_get_error(gdf_ipc_parser_type *handle);

/**
 * @brief  Gets parsed data from gdf_ipc_parser_type
 *
 * @param[in] Pointer to a parsing struct gdf_ipc_parser_type
 *
 * @returns Pointer parsed data
 */
const void* gdf_ipc_parser_get_data(gdf_ipc_parser_type *handle);

/**
 * @brief  Gets data offset from gdf_ipc_parser_type
 *
 * @param[in] Pointer to a parsing struct gdf_ipc_parser_type
 *
 * @returns Data offset
 */
int64_t gdf_ipc_parser_get_data_offset(gdf_ipc_parser_type *handle);

/**
 * @brief  returns parsed schema as json
 *
 * @param[in] Pointer to a parsing struct gdf_ipc_parser_type
 *
 * @returns char* of parsed schema as json
 */
const char *gdf_ipc_parser_get_schema_json(gdf_ipc_parser_type *handle);

/**
 * @brief  returns layout as json
 *
 * @param[in] Pointer to a parsing struct gdf_ipc_parser_type
 *
 * @returns char* of layout as json
 */
const char *gdf_ipc_parser_get_layout_json(gdf_ipc_parser_type *handle);


/* sorting */

/**
 * @brief  Constructor for the gdf_radixsort_plan_type object
 *
 * @param[in] Number of items to sort
 * @param[in] Indicates if sort should be ascending or descending. 1 = Descending, 0 = Ascending
 * @param[in] The least-significant bit index (inclusive) needed for key comparison
 * @param[in] The most-significant bit index (exclusive) needed for key comparison (e.g., sizeof(unsigned int) * 8)
 *
 * @returns  gdf_radixsort_plan_type object pointer
 */
gdf_radixsort_plan_type* gdf_radixsort_plan(size_t num_items, int descending,
                                        unsigned begin_bit, unsigned end_bit);

/**
 * @brief  Allocates device memory for the radixsort
 *
 * @param[in] Radix sort plan
 * @param[in] sizeof data type of key
 * @param[in] sizeof data type of val
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_radixsort_plan_setup(gdf_radixsort_plan_type *hdl,
                                   size_t sizeof_key, size_t sizeof_val);

/**
 * @brief  Frees device memory used for the radixsort
 *
 * @param[in] Radix sort plan
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_radixsort_plan_free(gdf_radixsort_plan_type *hdl);


/*
 * The following function performs a sort on the key and value columns.
 * The null_count of the keycol and valcol columns are expected to be 0
 * otherwise a GDF_VALIDITY_UNSUPPORTED error is returned.
 */

/**
 * @brief  Performs a radixsort on the key and value columns where the key is an int8
 *
 * @param[in] Radix sort plan
 * @param[in] key gdf_column
 * @param[in] value gdf_column
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_radixsort_i8(gdf_radixsort_plan_type *hdl,
                           gdf_column *keycol,
                           gdf_column *valcol);

/**
 * @brief  Performs a radixsort on the key and value columns where the key is an int32
 *
 * @param[in] Radix sort plan
 * @param[in] key gdf_column
 * @param[in] value gdf_column
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_radixsort_i32(gdf_radixsort_plan_type *hdl,
                            gdf_column *keycol,
                            gdf_column *valcol);

/**
 * @brief  performs a radixsort on the key and value columns where the key is an int64
 *
 * @param[in] Radix sort plan
 * @param[in] key gdf_column
 * @param[in] value gdf_column
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_radixsort_i64(gdf_radixsort_plan_type *hdl,
                            gdf_column *keycol,
                            gdf_column *valcol);

/**
 * @brief  performs a radixsort on the key and value columns where the key is an float
 *
 * @param[in] Radix sort plan
 * @param[in] key gdf_column
 * @param[in] value gdf_column
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_radixsort_f32(gdf_radixsort_plan_type *hdl,
                            gdf_column *keycol,
                            gdf_column *valcol);

/**
 * @brief  performs a radixsort on the key and value columns where the key is an double
 *
 * @param[in] Radix sort plan
 * @param[in] key gdf_column
 * @param[in] value gdf_column
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_radixsort_f64(gdf_radixsort_plan_type *hdl,
                            gdf_column *keycol,
                            gdf_column *valcol);

/**
 * @brief  performs a radixsort on the key and value columns where the key is any type
 *
 * @param[in] Radix sort plan
 * @param[in] key gdf_column
 * @param[in] value gdf_column
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_radixsort_generic(gdf_radixsort_plan_type *hdl,
                                gdf_column *keycol,
                                gdf_column *valcol);


/* segmented sorting */

/**
 * @brief  Constructor for the gdf_segmented_radixsort_plan_type object
 *
 * @param[in] Number of items to sort
 * @param[in] Indicates if sort should be ascending or descending. 1 = Descending, 0 = Ascending
 * @param[in] The least-significant bit index (inclusive) needed for key comparison
 * @param[in] The most-significant bit index (exclusive) needed for key comparison (e.g., sizeof(unsigned int) * 8)
 *
 * @returns  gdf_segmented_radixsort_plan_type object pointer
 */
gdf_segmented_radixsort_plan_type* gdf_segmented_radixsort_plan(size_t num_items,
                                                                int descending,
                                                                unsigned begin_bit,
                                                                unsigned end_bit);

/**
 * @brief  Allocates device memory for the segmented radixsort
 *
 * @param[in] Segmented Radix sort plan
 * @param[in] sizeof data type of key
 * @param[in] sizeof data type of val
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_segmented_radixsort_plan_setup(gdf_segmented_radixsort_plan_type *hdl,
                                            size_t sizeof_key,
                                            size_t sizeof_val);

/**
 * @brief  Frees device memory used for the segmented radixsort
 *
 * @param[in] Segmented Radix sort plan
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_segmented_radixsort_plan_free(gdf_segmented_radixsort_plan_type *hdl);


/*
 * The following function performs a sort on the key and value columns.
 * The null_count of the keycol and valcol columns are expected to be 0
 * otherwise a GDF_VALIDITY_UNSUPPORTED error is returned.
 */

/**
 * @brief  performs a segmented radixsort on the key and value columns where the key is an int8
 *
 * @param[in] Radix sort plan
 * @param[in] key gdf_column
 * @param[in] value gdf_column
 * @param[in] The number of segments that comprise the sorting data
 * @param[in] Pointer to the sequence of beginning offsets of length num_segments, such that d_begin_offsets[i] is the first element of the ith data segment in d_keys_* and d_values_*
 * @param[in] Pointer to the sequence of ending offsets of length num_segments, such that d_end_offsets[i]-1 is the last element of the ith data segment in d_keys_* and d_values_*. If d_end_offsets[i]-1 <= d_begin_offsets[i], the ith is considered empty.
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_segmented_radixsort_i8(gdf_segmented_radixsort_plan_type *hdl,
                                     gdf_column *keycol, gdf_column *valcol,
                                     unsigned num_segments,
                                     unsigned *d_begin_offsets,
                                     unsigned *d_end_offsets);

/**
 * @brief  performs a segmented radixsort on the key and value columns where the key is an int32
 *
 * @param[in] Radix sort plan
 * @param[in] key gdf_column
 * @param[in] value gdf_column
 * @param[in] The number of segments that comprise the sorting data
 * @param[in] Pointer to the sequence of beginning offsets of length num_segments, such that d_begin_offsets[i] is the first element of the ith data segment in d_keys_* and d_values_*
 * @param[in] Pointer to the sequence of ending offsets of length num_segments, such that d_end_offsets[i]-1 is the last element of the ith data segment in d_keys_* and d_values_*. If d_end_offsets[i]-1 <= d_begin_offsets[i], the ith is considered empty.
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_segmented_radixsort_i32(gdf_segmented_radixsort_plan_type *hdl,
                                     gdf_column *keycol, gdf_column *valcol,
                                     unsigned num_segments,
                                     unsigned *d_begin_offsets,
                                     unsigned *d_end_offsets);

/**
 * @brief  performs a segmented radixsort on the key and value columns where the key is an int64
 *
 * @param[in] Radix sort plan
 * @param[in] key gdf_column
 * @param[in] value gdf_column
 * @param[in] The number of segments that comprise the sorting data
 * @param[in] Pointer to the sequence of beginning offsets of length num_segments, such that d_begin_offsets[i] is the first element of the ith data segment in d_keys_* and d_values_*
 * @param[in] Pointer to the sequence of ending offsets of length num_segments, such that d_end_offsets[i]-1 is the last element of the ith data segment in d_keys_* and d_values_*. If d_end_offsets[i]-1 <= d_begin_offsets[i], the ith is considered empty.
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_segmented_radixsort_i64(gdf_segmented_radixsort_plan_type *hdl,
                                     gdf_column *keycol, gdf_column *valcol,
                                     unsigned num_segments,
                                     unsigned *d_begin_offsets,
                                     unsigned *d_end_offsets);

/**
 * @brief  performs a segmented radixsort on the key and value columns where the key is an float
 *
 * @param[in] Radix sort plan
 * @param[in] key gdf_column
 * @param[in] value gdf_column
 * @param[in] The number of segments that comprise the sorting data
 * @param[in] Pointer to the sequence of beginning offsets of length num_segments, such that d_begin_offsets[i] is the first element of the ith data segment in d_keys_* and d_values_*
 * @param[in] Pointer to the sequence of ending offsets of length num_segments, such that d_end_offsets[i]-1 is the last element of the ith data segment in d_keys_* and d_values_*. If d_end_offsets[i]-1 <= d_begin_offsets[i], the ith is considered empty.
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_segmented_radixsort_f32(gdf_segmented_radixsort_plan_type *hdl,
                                     gdf_column *keycol, gdf_column *valcol,
                                     unsigned num_segments,
                                     unsigned *d_begin_offsets,
                                     unsigned *d_end_offsets);
                                    
/**
 * @brief  performs a segmented radixsort on the key and value columns where the key is an double
 *
 * @param[in] Radix sort plan
 * @param[in] key gdf_column
 * @param[in] value gdf_column
 * @param[in] The number of segments that comprise the sorting data
 * @param[in] Pointer to the sequence of beginning offsets of length num_segments, such that d_begin_offsets[i] is the first element of the ith data segment in d_keys_* and d_values_*
 * @param[in] Pointer to the sequence of ending offsets of length num_segments, such that d_end_offsets[i]-1 is the last element of the ith data segment in d_keys_* and d_values_*. If d_end_offsets[i]-1 <= d_begin_offsets[i], the ith is considered empty.
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_segmented_radixsort_f64(gdf_segmented_radixsort_plan_type *hdl,
                                     gdf_column *keycol, gdf_column *valcol,
                                     unsigned num_segments,
                                     unsigned *d_begin_offsets,
                                     unsigned *d_end_offsets);
                                    
/**
 * @brief  performs a segmented radixsort on the key and value columns where the key is any type
 *
 * @param[in] Radix sort plan
 * @param[in] key gdf_column
 * @param[in] value gdf_column
 * @param[in] The number of segments that comprise the sorting data
 * @param[in] Pointer to the sequence of beginning offsets of length num_segments, such that d_begin_offsets[i] is the first element of the ith data segment in d_keys_* and d_values_*
 * @param[in] Pointer to the sequence of ending offsets of length num_segments, such that d_end_offsets[i]-1 is the last element of the ith data segment in d_keys_* and d_values_*. If d_end_offsets[i]-1 <= d_begin_offsets[i], the ith is considered empty.
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_segmented_radixsort_generic(gdf_segmented_radixsort_plan_type *hdl,
                                     gdf_column *keycol, gdf_column *valcol,
                                     unsigned num_segments,
                                     unsigned *d_begin_offsets,
                                     unsigned *d_end_offsets);


// transpose
/**
 * @brief Transposes the table in_cols and copies to out_cols
 * 
 * @param[in] ncols Number of columns in in_cols
 * @param[in] in_cols[] Input table of (ncols) number of columns each of size (nrows)
 * @param[out] out_cols[] Preallocated output_table of (nrows) columns each of size (ncols)
 * @returns gdf_error GDF_SUCCESS if successful, else appropriate error code
 */
gdf_error gdf_transpose(gdf_size_type ncols,
                        gdf_column** in_cols,
                        gdf_column** out_cols);

// joins

/** 
 * @brief  Performs an inner join on the specified columns of two
 * dataframes (left, right)
 * If join_context->flag_method is set to GDF_SORT then the null_count of the
 * columns must be set to 0 otherwise a GDF_VALIDITY_UNSUPPORTED error is
 * returned.
 * 
 * @param[in] left_cols[] The columns of the left dataframe
 * @param[in] num_left_cols The number of columns in the left dataframe
 * @param[in] left_join_cols[] The column indices of columns from the left dataframe
 * to join on
 * @param[in] right_cols[] The columns of the right dataframe
 * @param[in] num_right_cols The number of columns in the right dataframe
 * @param[in] right_join_cols[] The column indices of columns from the right dataframe
 * to join on
 * @param[in] num_cols_to_join The total number of columns to join on
 * @param[in] result_num_cols The number of columns in the resulting dataframe
 * @param[out] gdf_column *result_cols[] If not nullptr, the dataframe that results from joining
 * the left and right tables on the specified columns
 * @param[out] gdf_column * left_indices If not nullptr, indices of rows from the left table that match rows in the right table
 * @param[out] gdf_column * right_indices If not nullptr, indices of rows from the right table that match rows in the left table
 * @param[in] join_context The context to use to control how the join is performed,e.g.,
 * sort vs hash based implementation
 * 
 * @returns   GDF_SUCCESS if the join operation was successful, otherwise an appropriate
 * error code
 */
gdf_error gdf_inner_join(
                         gdf_column **left_cols, 
                         int num_left_cols,
                         int left_join_cols[],
                         gdf_column **right_cols,
                         int num_right_cols,
                         int right_join_cols[],
                         int num_cols_to_join,
                         int result_num_cols,
                         gdf_column **result_cols,
                         gdf_column * left_indices,
                         gdf_column * right_indices,
                         gdf_context *join_context);

/** 
 * @brief  Performs a left join (also known as left outer join) on the
 * specified columns of two dataframes (left, right)
 * If join_context->flag_method is set to GDF_SORT then the null_count of the
 * columns must be set to 0 otherwise a GDF_VALIDITY_UNSUPPORTED error is
 * returned.
 * 
 * @param[in] left_cols[] The columns of the left dataframe
 * @param[in] num_left_cols The number of columns in the left dataframe
 * @param[in] left_join_cols[] The column indices of columns from the left dataframe
 * to join on
 * @param[in] right_cols[] The columns of the right dataframe
 * @param[in] num_right_cols The number of columns in the right dataframe
 * @param[in] right_join_cols[] The column indices of columns from the right dataframe
 * to join on
 * @param[in] num_cols_to_join The total number of columns to join on
 * @param[in] result_num_cols The number of columns in the resulting dataframe
 * @param[out] gdf_column *result_cols[] If not nullptr, the dataframe that results from joining
 * the left and right tables on the specified columns
 * @param[out] gdf_column * left_indices If not nullptr, indices of rows from the left table that match rows in the right table
 * @param[out] gdf_column * right_indices If not nullptr, indices of rows from the right table that match rows in the left table
 * @param[in] join_context The context to use to control how the join is performed,e.g.,
 * sort vs hash based implementation
 * 
 * @returns   GDF_SUCCESS if the join operation was successful, otherwise an appropriate
 * error code
 */
gdf_error gdf_left_join(
                         gdf_column **left_cols, 
                         int num_left_cols,
                         int left_join_cols[],
                         gdf_column **right_cols,
                         int num_right_cols,
                         int right_join_cols[],
                         int num_cols_to_join,
                         int result_num_cols,
                         gdf_column **result_cols,
                         gdf_column * left_indices,
                         gdf_column * right_indices,
                         gdf_context *join_context);

/** 
 * @brief  Performs a full join (also known as full outer join) on the
 * specified columns of two dataframes (left, right)
 * If join_context->flag_method is set to GDF_SORT then the null_count of the
 * columns must be set to 0 otherwise a GDF_VALIDITY_UNSUPPORTED error is
 * returned.
 * 
 * @param[in] left_cols[] The columns of the left dataframe
 * @param[in] num_left_cols The number of columns in the left dataframe
 * @param[in] left_join_cols[] The column indices of columns from the left dataframe
 * to join on
 * @param[in] right_cols[] The columns of the right dataframe
 * @param[in] num_right_cols The number of columns in the right dataframe
 * @param[in] right_join_cols[] The column indices of columns from the right dataframe
 * to join on
 * @param[in] num_cols_to_join The total number of columns to join on
 * @param[in] result_num_cols The number of columns in the resulting dataframe
 * @param[out] gdf_column *result_cols[] If not nullptr, the dataframe that results from joining
 * the left and right tables on the specified columns
 * @param[out] gdf_column * left_indices If not nullptr, indices of rows from the left table that match rows in the right table
 * @param[out] gdf_column * right_indices If not nullptr, indices of rows from the right table that match rows in the left table
 * @param[in] join_context The context to use to control how the join is performed,e.g.,
 * sort vs hash based implementation
 * 
 * @returns   GDF_SUCCESS if the join operation was successful, otherwise an appropriate
 * error code
 */
gdf_error gdf_full_join(
                         gdf_column **left_cols, 
                         int num_left_cols,
                         int left_join_cols[],
                         gdf_column **right_cols,
                         int num_right_cols,
                         int right_join_cols[],
                         int num_cols_to_join,
                         int result_num_cols,
                         gdf_column **result_cols,
                         gdf_column * left_indices,
                         gdf_column * right_indices,
                         gdf_context *join_context);

/* partioning */

/** 
 * @brief Computes the hash values of the rows in the specified columns of the 
 * input columns and bins the hash values into the desired number of partitions. 
 * Rearranges the input columns such that rows with hash values in the same bin 
 * are contiguous.
 * 
 * @param[in] num_input_cols The number of columns in the input columns
 * @param[in] input[] The input set of columns
 * @param[in] columns_to_hash[] Indices of the columns in the input set to hash
 * @param[in] num_cols_to_hash The number of columns to hash
 * @param[in] num_partitions The number of partitions to rearrange the input rows into
 * @param[out] partitioned_output Preallocated gdf_columns to hold the rearrangement 
 * of the input columns into the desired number of partitions
 * @param[out] partition_offsets Preallocated array the size of the number of
 * partitions. Where partition_offsets[i] indicates the starting position
 * of partition 'i'
 * @param[in] hash The hash function to use
 * 
 * @returns  If the operation was successful, returns GDF_SUCCESS
 */
gdf_error gdf_hash_partition(int num_input_cols, 
                             gdf_column * input[], 
                             int columns_to_hash[],
                             int num_cols_to_hash,
                             int num_partitions, 
                             gdf_column * partitioned_output[],
                             int partition_offsets[],
                             gdf_hash_func hash);

/* prefixsum */
/** --------------------------------------------------------------------------*
 * @brief  Computes the prefix sum of a column.
 * The null values are skipped for the operation, and if an input element
 * at `i` is null, then the output element at `i` will also be null.
 *
 * @param[in] input The input column for prefix sum
 * @param[out] output The pre-allocated output column containing
 * the prefix sum of the input
 * @param[in] inclusive The flag for applying an inclusive prefix sum if true,
 * an exclusive prefix sum if false.
 *
 * @returns   GDF_SUCCESS if the operation was successful,
 * otherwise an appropriate error code.
 * GDF_UNSUPPORTED_DTYPE if input->dtype is not an arithmetic type.
 * ----------------------------------------------------------------------------**/
gdf_error gdf_prefixsum(const gdf_column *input, gdf_column *output, bool inclusive);

/* unary operators */

/* hashing */
/** --------------------------------------------------------------------------*
 * @brief Computes the hash value of each row in the input set of columns.
 *
 * @param[in] num_cols The number of columns in the input set
 * @param[in] input The list of columns whose rows will be hashed
 * @param[in] hash The hash function to use
 * @param[in] initial_hash_values Optional array in device memory specifying an initial hash value for each column
 * that will be combined with the hash of every element in the column. If this argument is `nullptr`,
 * then each element will be hashed as-is.
 * @param[out] output The hash value of each row of the input
 *
 * @returns    GDF_SUCCESS if the operation was successful, otherwise an
 *            appropriate error code.
 * ----------------------------------------------------------------------------**/
gdf_error gdf_hash(int num_cols,
                   gdf_column **input,
                   gdf_hash_func hash,
                   uint32_t *initial_hash_values,
                   gdf_column *output);

/* trig */

/**
 * @brief  Computes trigonometric sine function for any floating point data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_sin_generic(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes trigonometric sine function for float data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_sin_f32(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes trigonometric sine function for double data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_sin_f64(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes trigonometric cosine function for any floating point data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cos_generic(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes trigonometric cosine function for float data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cos_f32(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes trigonometric cosine function for double data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cos_f64(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes trigonometric tangent function for any floating point data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_tan_generic(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes trigonometric tangent function for float data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_tan_f32(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes trigonometric tangent function for double data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_tan_f64(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes trigonometric arcsin function for any floating point data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_asin_generic(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes trigonometric arcsin function for float data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_asin_f32(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes trigonometric arcsin function for double data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_asin_f64(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes trigonometric arccos function for any floating point data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_acos_generic(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes trigonometric arccos function for float data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_acos_f32(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes trigonometric arccos function for double data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_acos_f64(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes trigonometric arctan function for any floating point data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_atan_generic(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes trigonometric arctan function for a float data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_atan_f32(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes trigonometric arctan function for a double data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_atan_f64(gdf_column *input, gdf_column *output);


/* exponential */

/**
 * @brief  Computes e (Euler's number, 2.7182818...) raised to the given power arg for any floating point data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_exp_generic(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes e (Euler's number, 2.7182818...) raised to the given power arg float data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_exp_f32(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes e (Euler's number, 2.7182818...) raised to the given power arg for double data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_exp_f64(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes the natural (base e) logarithm of arg for any floating point data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_log_generic(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes the natural (base e) logarithm of arg for float data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_log_f32(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes the natural (base e) logarithm of arg for double data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_log_f64(gdf_column *input, gdf_column *output);


/* power */

/**
 * @brief  Computes the square root for any floating point data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_sqrt_generic(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes the square root for float data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_sqrt_f32(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes the square root for double data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_sqrt_f64(gdf_column *input, gdf_column *output);


/* rounding */

/**
 * @brief  Computes the smallest integer value not less than arg for any floating point data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_ceil_generic(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes the smallest integer value not less than arg for float data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_ceil_f32(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes the smallest integer value not less than arg for double data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_ceil_f64(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes the largest integer value not greater than arg for any floating point data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_floor_generic(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes the largest integer value not greater than arg for float data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_floor_f32(gdf_column *input, gdf_column *output);

/**
 * @brief  Computes the largest integer value not greater than arg for double data type
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_floor_f64(gdf_column *input, gdf_column *output);


/* casting */

/**
 * @brief  Casts data in a gdf_column of any data type to a GDF_FLOAT32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_generic_to_f32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column of type GDF_INT8 to a GDF_FLOAT32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_i8_to_f32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column of type GDF_INT32 to a GDF_FLOAT32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_i32_to_f32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column of type GDF_INT64 to a GDF_FLOAT32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_i64_to_f32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column of type GDF_FLOAT32 to a GDF_FLOAT32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_f32_to_f32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column of type GDF_FLOAT64 to a GDF_FLOAT32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_f64_to_f32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column of type GDF_DATE32 to a GDF_FLOAT32
 *
 * This is effectively casting the underlying GDF_INT32 physical data type of GDF_DATE32 to GDF_FLOAT32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_date32_to_f32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column of type GDF_DATE64 to a GDF_FLOAT32
 *
 * This is effectively casting the underlying GDF_INT64 physical data type of GDF_DATE64 to GDF_FLOAT32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_date64_to_f32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column of type GDF_TIMESTAMP to a GDF_FLOAT32
 *
 * This is effectively casting the underlying GDF_INT64 physical data type of GDF_TIMESTAMP to GDF_FLOAT32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_timestamp_to_f32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column of any data type to a GDF_FLOAT64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_generic_to_f64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_INT8 to a GDF_FLOAT64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_i8_to_f64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_INT32 to a GDF_FLOAT64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_i32_to_f64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_INT64 to a GDF_FLOAT64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_i64_to_f64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_FLOAT32 to a GDF_FLOAT64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_f32_to_f64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_FLOAT64 to a GDF_FLOAT64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_f64_to_f64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_DATE32 to a GDF_FLOAT64
 *
 * This is effectively casting the underlying GDF_INT32 physical data type of GDF_DATE32 to GDF_FLOAT64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_date32_to_f64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_DATE64 to a GDF_FLOAT64
 *
 * This is effectively casting the underlying GDF_INT64 physical data type of GDF_DATE64 to GDF_FLOAT64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_date64_to_f64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_TIMESTAMP to a GDF_FLOAT64
 *
 * This is effectively casting the underlying GDF_INT64 physical data type of GDF_TIMESTAMP to GDF_FLOAT64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_timestamp_to_f64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column of any data type to a GDF_INT8
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_generic_to_i8(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_INT8 to a GDF_INT8
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_i8_to_i8(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_INT32 to a GDF_INT8
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_i32_to_i8(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_INT64 to a GDF_INT8
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_i64_to_i8(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_FLOAT32 to a GDF_INT8
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_f32_to_i8(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_FLOAT64 to a GDF_INT8
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_f64_to_i8(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_DATE32 to a GDF_INT8
 *
 * This is effectively casting the underlying GDF_INT32 physical data type of GDF_DATE32 to GDF_INT8
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_date32_to_i8(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_DATE64 to a GDF_INT8
 *
 * This is effectively casting the underlying GDF_INT64 physical data type of GDF_DATE64 to GDF_INT8
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_date64_to_i8(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_TIMESTAMP to a GDF_INT8
 *
 * This is effectively casting the underlying GDF_INT64 physical data type of GDF_TIMESTAMP to GDF_INT8
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_timestamp_to_i8(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column of any data type to a GDF_INT32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_generic_to_i32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_INT8 to a GDF_INT32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_i8_to_i32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_INT32 to a GDF_INT32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_i32_to_i32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_INT64 to a GDF_INT32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_i64_to_i32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_FLOAT32 to a GDF_INT32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_f32_to_i32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_FLOAT64 to a GDF_INT32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_f64_to_i32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_DATE32 to a GDF_INT32
 *
 * This is effectively casting the underlying GDF_INT32 physical data type of GDF_DATE32 to GDF_INT32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_date32_to_i32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_DATE64 to a GDF_INT32
 *
 * This is effectively casting the underlying GDF_INT64 physical data type of GDF_DATE64 to GDF_INT32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_date64_to_i32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_TIMESTAMP to a GDF_INT32
 *
 * This is effectively casting the underlying GDF_INT64 physical data type of GDF_TIMESTAMP to GDF_INT32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_timestamp_to_i32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column of any data type to a GDF_INT64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_generic_to_i64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_INT8 to a GDF_INT64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_i8_to_i64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_INT32 to a GDF_INT64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_i32_to_i64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_INT64 to a GDF_INT64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_i64_to_i64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_FLOAT32 to a GDF_INT64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_f32_to_i64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_FLOAT64 to a GDF_INT64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_f64_to_i64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_DATE32 to a GDF_INT64
 *
 * This is effectively casting the underlying GDF_INT32 physical data type of GDF_DATE32 to GDF_INT64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_date32_to_i64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_DATE64 to a GDF_INT64
 *
 * This is effectively casting the underlying GDF_INT64 physical data type of GDF_DATE64 to GDF_INT64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_date64_to_i64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_TIMESTAMP to a GDF_INT64
 *
 * This is effectively casting the underlying GDF_INT64 physical data type of GDF_TIMESTAMP to GDF_INT64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_timestamp_to_i64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column of any data type to a GDF_DATE32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_generic_to_date32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_INT8 to a GDF_DATE32
 *
 * This is effectively casting the GDF_INT8 to the underlying GDF_INT32 physical data type of GDF_DATE32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_i8_to_date32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_INT32 to a GDF_DATE32
 *
 * This is effectively casting the GDF_INT32 to the underlying GDF_INT32 physical data type of GDF_DATE32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_i32_to_date32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_INT64 to a GDF_DATE32
 *
 * This is effectively casting the GDF_INT64 to the underlying GDF_INT32 physical data type of GDF_DATE32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_i64_to_date32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_FLOAT32 to a GDF_DATE32
 *
 * This is effectively casting the GDF_FLOAT32 to the underlying GDF_INT32 physical data type of GDF_DATE32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_f32_to_date32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_FLOAT64 to a GDF_DATE32
 *
 * This is effectively casting the GDF_FLOAT64 to the underlying GDF_INT32 physical data type of GDF_DATE32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_f64_to_date32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_DATE32 to a GDF_DATE32
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_date32_to_date32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_DATE64 to a GDF_DATE32
 *
 * This casting converts from milliseconds since the UNIX epoch to days since the UNIX epoch
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_date64_to_date32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_TIMESTAMP to a GDF_DATE32
 *
 * This casting converts from gdf_time_unit since the UNIX epoch to days since the UNIX epoch
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_timestamp_to_date32(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column of any data type to a GDF_FLOAT64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_generic_to_date64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_INT8 to a GDF_DATE64
 *
 * This is effectively casting the GDF_INT8 to the underlying GDF_INT64 physical data type of GDF_DATE64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_i8_to_date64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_INT32 to a GDF_DATE64
 *
 * This is effectively casting the GDF_INT32 to the underlying GDF_INT64 physical data type of GDF_DATE64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_i32_to_date64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_INT64 to a GDF_DATE64
 *
 * This is effectively casting the GDF_INT64 to the underlying GDF_INT64 physical data type of GDF_DATE64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_i64_to_date64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_FLOAT32 to a GDF_DATE64
 *
 * This is effectively casting the GDF_FLOAT32 to the underlying GDF_INT64 physical data type of GDF_DATE64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_f32_to_date64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_FLOAT64 to a GDF_DATE64
 *
 * This is effectively casting the GDF_FLOAT64 to the underlying GDF_INT64 physical data type of GDF_DATE64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_f64_to_date64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_DATE32 to a GDF_DATE64
 *
 * This casting converts from days since the UNIX epoch to milliseconds since the UNIX epoch
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_date32_to_date64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_DATE64 to a GDF_DATE64
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_date64_to_date64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column type GDF_TIMESTAMP to a GDF_DATE32
 *
 * This casting converts from gdf_time_unit since the UNIX epoch to milliseconds since the UNIX epoch
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_timestamp_to_date64(gdf_column *input, gdf_column *output);

/**
 * @brief  Casts data in a gdf_column of any data type to a GDF_TIMESTAMP
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_generic_to_timestamp(gdf_column *input, gdf_column *output, gdf_time_unit time_unit);

/**
 * @brief  Casts data in a gdf_column type GDF_INT8 to a GDF_TIMESTAMP
 *
 * This is effectively casting the GDF_INT8 to the underlying GDF_INT64 physical data type of GDF_TIMESTAMP
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_i8_to_timestamp(gdf_column *input, gdf_column *output, gdf_time_unit time_unit);

/**
 * @brief  Casts data in a gdf_column type GDF_INT32 to a GDF_TIMESTAMP
 *
 * This is effectively casting the GDF_INT32 to the underlying GDF_INT64 physical data type of GDF_TIMESTAMP
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_i32_to_timestamp(gdf_column *input, gdf_column *output, gdf_time_unit time_unit);

/**
 * @brief  Casts data in a gdf_column type GDF_INT64 to a GDF_TIMESTAMP
 *
 * This is effectively casting the GDF_INT64 to the underlying GDF_INT64 physical data type of GDF_TIMESTAMP
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_i64_to_timestamp(gdf_column *input, gdf_column *output, gdf_time_unit time_unit);

/**
 * @brief  Casts data in a gdf_column type GDF_FLOAT32 to a GDF_TIMESTAMP
 *
 * This is effectively casting the GDF_FLOAT32 to the underlying GDF_INT64 physical data type of GDF_TIMESTAMP
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_f32_to_timestamp(gdf_column *input, gdf_column *output, gdf_time_unit time_unit);

/**
 * @brief  Casts data in a gdf_column type GDF_FLOAT64 to a GDF_TIMESTAMP
 *
 * This is effectively casting the GDF_FLOAT64 to the underlying GDF_INT64 physical data type of GDF_TIMESTAMP
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_f64_to_timestamp(gdf_column *input, gdf_column *output, gdf_time_unit time_unit);

/**
 * @brief  Casts data in a gdf_column type GDF_DATE32 to a GDF_TIMESTAMP
 *
 * This casting converts from days since UNIX epoch to gdf_time_unit since the UNIX epoch
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_date32_to_timestamp(gdf_column *input, gdf_column *output, gdf_time_unit time_unit);

/**
 * @brief  Casts data in a gdf_column type GDF_DATE64 to a GDF_TIMESTAMP
 *
 * This casting converts from milliseconds days since UNIX epoch to gdf_time_unit since the UNIX epoch
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_date64_to_timestamp(gdf_column *input, gdf_column *output, gdf_time_unit time_unit);

/**
 * @brief  Casts data in a gdf_column type GDF_TIMESTAMP to a GDF_TIMESTAMP
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_cast_timestamp_to_timestamp(gdf_column *input, gdf_column *output, gdf_time_unit time_unit);


/* datetime extract*/

/**
 * @brief  Extracts year from any date time type and places results into a preallocated GDF_INT16 column
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_extract_datetime_year(gdf_column *input, gdf_column *output);

/**
 * @brief  Extracts month from any date time type and places results into a preallocated GDF_INT16 column
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_extract_datetime_month(gdf_column *input, gdf_column *output);

/**
 * @brief  Extracts day from any date time type and places results into a preallocated GDF_INT16 column
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_extract_datetime_day(gdf_column *input, gdf_column *output);

/**
 * @brief  Extracts hour from either GDF_DATE64 or GDF_TIMESTAMP type and places results into a preallocated GDF_INT16 column
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_extract_datetime_hour(gdf_column *input, gdf_column *output);

/**
 * @brief  Extracts minute from either GDF_DATE64 or GDF_TIMESTAMP type and places results into a preallocated GDF_INT16 column
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_extract_datetime_minute(gdf_column *input, gdf_column *output);

/**
 * @brief  Extracts second from either GDF_DATE64 or GDF_TIMESTAMP type and places results into a preallocated GDF_INT16 column
 *
 * @param[in] gdf_column of the input
 * @param[out] output gdf_column. The output memory needs to be preallocated
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_extract_datetime_second(gdf_column *input, gdf_column *output);


/* binary operators */

/* arith */

gdf_error gdf_add_generic(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_add_i32(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_add_i64(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_add_f32(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_add_f64(gdf_column *lhs, gdf_column *rhs, gdf_column *output);

gdf_error gdf_sub_generic(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_sub_i32(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_sub_i64(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_sub_f32(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_sub_f64(gdf_column *lhs, gdf_column *rhs, gdf_column *output);

gdf_error gdf_mul_generic(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_mul_i32(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_mul_i64(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_mul_f32(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_mul_f64(gdf_column *lhs, gdf_column *rhs, gdf_column *output);

gdf_error gdf_floordiv_generic(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_floordiv_i32(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_floordiv_i64(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_floordiv_f32(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_floordiv_f64(gdf_column *lhs, gdf_column *rhs, gdf_column *output);

gdf_error gdf_div_generic(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_div_f32(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_div_f64(gdf_column *lhs, gdf_column *rhs, gdf_column *output);

/* logical */

gdf_error gdf_gt_generic(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_gt_i8(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_gt_i32(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_gt_i64(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_gt_f32(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_gt_f64(gdf_column *lhs, gdf_column *rhs, gdf_column *output);

gdf_error gdf_ge_generic(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_ge_i8(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_ge_i32(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_ge_i64(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_ge_f32(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_ge_f64(gdf_column *lhs, gdf_column *rhs, gdf_column *output);

gdf_error gdf_lt_generic(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_lt_i8(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_lt_i32(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_lt_i64(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_lt_f32(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_lt_f64(gdf_column *lhs, gdf_column *rhs, gdf_column *output);

gdf_error gdf_le_generic(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_le_i8(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_le_i32(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_le_i64(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_le_f32(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_le_f64(gdf_column *lhs, gdf_column *rhs, gdf_column *output);

gdf_error gdf_eq_generic(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_eq_i8(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_eq_i32(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_eq_i64(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_eq_f32(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_eq_f64(gdf_column *lhs, gdf_column *rhs, gdf_column *output);

gdf_error gdf_ne_generic(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_ne_i8(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_ne_i32(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_ne_i64(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_ne_f32(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_ne_f64(gdf_column *lhs, gdf_column *rhs, gdf_column *output);

/* bitwise */

gdf_error gdf_bitwise_and_generic(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_bitwise_and_i8(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_bitwise_and_i32(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_bitwise_and_i64(gdf_column *lhs, gdf_column *rhs, gdf_column *output);

gdf_error gdf_bitwise_or_generic(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_bitwise_or_i8(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_bitwise_or_i32(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_bitwise_or_i64(gdf_column *lhs, gdf_column *rhs, gdf_column *output);


/*
 * Filtering and comparison operators
 */

gdf_error gdf_bitwise_xor_generic(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_bitwise_xor_i8(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_bitwise_xor_i32(gdf_column *lhs, gdf_column *rhs, gdf_column *output);
gdf_error gdf_bitwise_xor_i64(gdf_column *lhs, gdf_column *rhs, gdf_column *output);

/* validity */

gdf_error gdf_validity_and(gdf_column *lhs, gdf_column *rhs, gdf_column *output);

/* reductions

The following reduction functions use the result array as a temporary working
space.  Use gdf_reduction_get_intermediate_output_size() to get the necessary
size for this use.
*/


/**
 * @brief  Reports the intermediate buffer size in elements required for 
 *         all cuDF reduction operations (gdf_sum, gdf_product, 
 *         gdf_sum_of_squares, gdf_min and gdf_max)
 *
 * @returns  The size of output/intermediate buffer to allocate for reductions
 * 
 * @todo Reductions should be re-implemented to use an atomic add for each
 *       block sum rather than launch a second kernel. When that happens, this
 *       function can go away and the output can be a single element.
 */
unsigned int gdf_reduction_get_intermediate_output_size();

/**
 * @brief  Computes the sum of the values in all rows of a column
 * 
 * @param[in] col Input column
 * @param[out] dev_result The output sum 
 * @param[in] dev_result_size The size of dev_result in elements, which should
 *                            be computed using gdf_reduction_get_intermediate_output_size
 *                            This is used as intermediate storage, and the 
 *                            first element contains the total result
 * 
 * @returns    GDF_SUCCESS if the operation was successful, otherwise an 
 *            appropriate error code. 
 * 
 */
gdf_error gdf_sum(gdf_column *col, void *dev_result, gdf_size_type dev_result_size);

/**
 * @brief  Computes the multiplicative product of the values in all rows of 
 *         a column
 * 
 * @param[in] col Input column
 * @param[out] dev_result The output product
 * @param[in] dev_result_size The size of dev_result in elements, which should
 *                            be computed using gdf_reduction_get_intermediate_output_size
 *                            This is used as intermediate storage, and the 
 *                            first element contains the total result
 * 
 * @returns    GDF_SUCCESS if the operation was successful, otherwise an 
 *            appropriate error code. 
 */
gdf_error gdf_product(gdf_column *col, void *dev_result, gdf_size_type dev_result_size);

/**
 * @brief  Computes the sum of squares of the values in all rows of a column
 * 
 * Sum of squares is useful for variance implementation.
 * 
 * @param[in] col Input column
 * @param[out] dev_result The output sum of squares
 * @param[in] dev_result_size The size of dev_result in elements, which should
 *                            be computed using gdf_reduction_get_intermediate_output_size
 *                            This is used as intermediate storage, and the 
 *                            first element contains the total result
 * 
 * @returns    GDF_SUCCESS if the operation was successful, otherwise an 
 *            appropriate error code. 
 * 
 * @todo could be implemented using inner_product if that function is 
 *       implemented
 */
gdf_error gdf_sum_of_squares(gdf_column *col, void *dev_result, gdf_size_type dev_result_size);

/**
 * @brief  Computes the minimum of the values in all rows of a column
 * 
 * @param[in] col Input column
 * @param[out] dev_result The output minimum
 * @param[in] dev_result_size The size of dev_result in elements, which should
 *                            be computed using gdf_reduction_get_intermediate_output_size
 *                            This is used as intermediate storage, and the 
 *                            first element contains the total result
 * 
 * @returns    GDF_SUCCESS if the operation was successful, otherwise an 
 *            appropriate error code. 
 * 
 */
gdf_error gdf_min(gdf_column *col, void *dev_result, gdf_size_type dev_result_size);

/**
 * @brief  Computes the maximum of the values in all rows of a column
 * 
 * @param[in] col Input column
 * @param[out] dev_result The output maximum
 * @param[in] dev_result_size The size of dev_result in elements, which should
 *                            be computed using gdf_reduction_get_intermediate_output_size
 *                            This is used as intermediate storage, and the 
 *                            first element contains the total result
 * 
 * @returns    GDF_SUCCESS if the operation was successful, otherwise an 
 *            appropriate error code. 
 * 
 */
gdf_error gdf_max(gdf_column *col, void *dev_result, gdf_size_type dev_result_size);


/*
 * Filtering and comparison operators
 */

/**
 * @brief  Compare every value on the left hand side to a static value and return a stencil in output which will have 1 when the comparison operation returns 1 and 0 otherwise
 *
 * @param[in] gdf_column of the input of type GDF_INT8
 * @param[in] Static value to compare against the input
 * @param[out] output gdf_column of type GDF_INT8. The output memory needs to be preallocated
 * @param[in] gdf_comparison_operator enum defining the comparison operator to be used
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_comparison_static_i8(gdf_column *lhs, int8_t value, gdf_column *output,gdf_comparison_operator operation);

/**
 * @brief  Compare every value on the left hand side to a static value and return a stencil in output which will have 1 when the comparison operation returns 1 and 0 otherwise
 *
 * @param[in] gdf_column of the input of type GDF_INT16
 * @param[in] Static value to compare against the input
 * @param[out] output gdf_column of type GDF_INT8. The output memory needs to be preallocated
 * @param[in] gdf_comparison_operator enum defining the comparison operator to be used
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_comparison_static_i16(gdf_column *lhs, int16_t value, gdf_column *output,gdf_comparison_operator operation);

/**
 * @brief  Compare every value on the left hand side to a static value and return a stencil in output which will have 1 when the comparison operation returns 1 and 0 otherwise
 *
 * @param[in] gdf_column of the input of type GDF_INT32
 * @param[in] Static value to compare against the input
 * @param[out] output gdf_column of type GDF_INT8. The output memory needs to be preallocated
 * @param[in] gdf_comparison_operator enum defining the comparison operator to be used
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_comparison_static_i32(gdf_column *lhs, int32_t value, gdf_column *output,gdf_comparison_operator operation);

/**
 * @brief  Compare every value on the left hand side to a static value and return a stencil in output which will have 1 when the comparison operation returns 1 and 0 otherwise
 *
 * @param[in] gdf_column of the input of type GDF_INT64
 * @param[in] Static value to compare against the input
 * @param[out] output gdf_column of type GDF_INT8. The output memory needs to be preallocated
 * @param[in] gdf_comparison_operator enum defining the comparison operator to be used
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_comparison_static_i64(gdf_column *lhs, int64_t value, gdf_column *output,gdf_comparison_operator operation);

/**
 * @brief  Compare every value on the left hand side to a static value and return a stencil in output which will have 1 when the comparison operation returns 1 and 0 otherwise
 *
 * @param[in] gdf_column of the input of type GDF_FLOAT32
 * @param[in] Static value to compare against the input
 * @param[out] output gdf_column of type GDF_INT8. The output memory needs to be preallocated
 * @param[in] gdf_comparison_operator enum defining the comparison operator to be used
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_comparison_static_f32(gdf_column *lhs, float value, gdf_column *output,gdf_comparison_operator operation);

/**
 * @brief  Compare every value on the left hand side to a static value and return a stencil in output which will have 1 when the comparison operation returns 1 and 0 otherwise
 *
 * @param[in] gdf_column of the input of type GDF_FLOAT64
 * @param[in] Static value to compare against the input
 * @param[out] output gdf_column of type GDF_INT8. The output memory needs to be preallocated
 * @param[in] gdf_comparison_operator enum defining the comparison operator to be used
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_comparison_static_f64(gdf_column *lhs, double value, gdf_column *output,gdf_comparison_operator operation);

/**
 * @brief  Compare two columns of any types against each other using a comparison operation, returns a stencil in output which will have 1 when the comparison operation returns 1 and 0 otherwise
 *
 * @param[in] gdf_column of one input of any type
 * @param[in] gdf_column of second input of any type
 * @param[out] output gdf_column of type GDF_INT8. The output memory needs to be preallocated
 * @param[in] gdf_comparison_operator enum defining the comparison operator to be used
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_comparison(gdf_column *lhs, gdf_column *rhs, gdf_column *output,gdf_comparison_operator operation);

/**
 * @brief  takes a stencil and uses it to compact a colum e.g. remove all values for which the stencil = 0
 *
 * @param[in] gdf_column of input of any type
 * @param[in] gdf_column holding the stencil
 * @param[out] output gdf_column of same type as input. The output memory needs to be preallocated to be the same size as input
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_apply_stencil(gdf_column *lhs, gdf_column * stencil, gdf_column * output);

/**
 * @brief  Concatenates two gdf_columns
 *
 * @param[in] gdf_column of one input of any type
 * @param[in] gdf_column of same type as the first
 * @param[out] output gdf_column of same type as inputs. The output memory needs to be preallocated to be the same size as the sum of both inputs
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_concat(gdf_column *lhs, gdf_column *rhs, gdf_column *output);


/*
 * Hashing
 */

/**
 * @brief  Creates a hash of multiple gdf_columns
 *
 * @param[in] an array of gdf_columns to be hashes together
 * @param[in] the number of columns in the array of gdf_columns to be hashes together
 * @param[out] output gdf_column of type GDF_INT64. The output memory needs to be preallocated
 * @param[in] A pointer to a cudaStream_t. If nullptr, the function will create a stream to use.
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_hash_columns(gdf_column ** columns_to_hash, int num_columns, gdf_column * output_column, void * stream);


/*
 * gdf introspection utlities
 */

/**
 * @brief  returns the byte width of the data type of the gdf_column
 *
 * @param[in] gdf_column whose data type's byte width will be determined
 * @param[out] the byte width of the data type
 *
 * @return gdf_error GDF_SUCCESS, or GDF_UNSUPPORTED_DTYPE if col has an invalid
 *         datatype
 */
gdf_error get_column_byte_width(gdf_column * col, int * width);


/* 
 Multi-Column SQL ops:
   WHERE (Filtering)
   ORDER-BY
   GROUP-BY
 */

/**
 * @brief  Performs SQL like WHERE (Filtering)
 *
 * @param[in] # rows
 * @param[in] host-side array of gdf_columns with 0 null_count otherwise GDF_VALIDITY_UNSUPPORTED is returned
 * @param[in] # cols
 * @param[out] pre-allocated device-side array to be filled with gdf_column::data for each column; slicing of gdf_column array (host)
 * @param[out] pre-allocated device-side array to be filled with gdf_colum::dtype for each column; slicing of gdf_column array (host)
 * @param[in] device-side array of values to filter against (type-erased)
 * @param[out] device-side array of row indices that remain after filtering
 * @param[out] host-side # rows that remain after filtering
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_filter(size_t nrows,
                    gdf_column* cols,
                    size_t ncols,
                    void** d_cols,
                    int* d_types, 
                    void** d_vals,
                    size_t* d_indx,
                    size_t* new_sz);

/**
 * @brief  Performs SQL like GROUP BY with SUM aggregation
 *
 * @param[in] # columns
 * @param[in] input cols
 * @param[in] column to aggregate on
 * @param[out] if not null return indices of re-ordered rows
 * @param[out] if not null return the grouped-by columns (multi-gather based on indices, which are needed anyway)
 * @param[out] aggregation result
 * @param[in] struct with additional info: bool is_sorted, flag_sort_or_hash, bool flag_count_distinct
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_group_by_sum(int ncols,
                           gdf_column** cols,
                           gdf_column* col_agg,
                           gdf_column* out_col_indices,
                           gdf_column** out_col_values,
                           gdf_column* out_col_agg,
                           gdf_context* ctxt); 

/**
 * @brief  Performs SQL like GROUP BY with MIN aggregation
 *
 * @param[in] # columns
 * @param[in] input cols
 * @param[in] column to aggregate on
 * @param[out] if not null return indices of re-ordered rows
 * @param[out] if not null return the grouped-by columns (multi-gather based on indices, which are needed anyway)
 * @param[out] aggregation result
 * @param[in] struct with additional info: bool is_sorted, flag_sort_or_hash, bool flag_count_distinct
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_group_by_min(int ncols,
                           gdf_column** cols,
                           gdf_column* col_agg,
                           gdf_column* out_col_indices,
                           gdf_column** out_col_values, 
                           gdf_column* out_col_agg,
                           gdf_context* ctxt);

/**
 * @brief  Performs SQL like GROUP BY with MAX aggregation
 *
 * @param[in] # columns
 * @param[in] input cols
 * @param[in] column to aggregate on
 * @param[out] if not null return indices of re-ordered rows
 * @param[out] if not null return the grouped-by columns (multi-gather based on indices, which are needed anyway)
 * @param[out] aggregation result
 * @param[in] struct with additional info: bool is_sorted, flag_sort_or_hash, bool flag_count_distinct
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_group_by_max(int ncols,
                           gdf_column** cols,
                           gdf_column* col_agg,
                           gdf_column* out_col_indices,
                           gdf_column** out_col_values,
                           gdf_column* out_col_agg,
                           gdf_context* ctxt);

/**
 * @brief  Performs SQL like GROUP BY with AVG aggregation
 *
 * @param[in] # columns
 * @param[in] input cols
 * @param[in] column to aggregate on
 * @param[out] if not null return indices of re-ordered rows
 * @param[out] if not null return the grouped-by columns (multi-gather based on indices, which are needed anyway)
 * @param[out] aggregation result
 * @param[in] struct with additional info: bool is_sorted, flag_sort_or_hash, bool flag_count_distinct
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_group_by_avg(int ncols,
                           gdf_column** cols,
                           gdf_column* col_agg,
                           gdf_column* out_col_indices,
                           gdf_column** out_col_values,
                           gdf_column* out_col_agg,
                           gdf_context* ctxt);

/**
 * @brief  Performs SQL like GROUP BY with COUNT aggregation
 *
 * @param[in] # columns
 * @param[in] input cols
 * @param[in] column to aggregate on
 * @param[out] if not null return indices of re-ordered rows
 * @param[out] if not null return the grouped-by columns (multi-gather based on indices, which are needed anyway)
 * @param[out] aggregation result
 * @param[in] struct with additional info: bool is_sorted, flag_sort_or_hash, bool flag_count_distinct
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_group_by_count(int ncols,
                             gdf_column** cols,
                             gdf_column* col_agg,
                             gdf_column* out_col_indices,
                             gdf_column** out_col_values,      
                             gdf_column* out_col_agg,
                             gdf_context* ctxt);

/**
 * @brief  Calculates exact quantiles
 *
 * @param[in] input column
 * @param[in] precision: type of quantile method calculation
 * @param[in] requested quantile in [0,1]
 * @param[out] result; for <exact> should probably be double*; it's void* because: (1) for uniformity of interface with <approx>; (2) for possible types bigger than double, in the future;
 * @param[in] struct with additional info
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_quantile_exact(gdf_column* col_in,
                            gdf_quantile_method prec,
                            double q,
                            void* t_erased_res,                            
                            gdf_context* ctxt);

/**
 * @brief  Calculates approximate quantiles
 *
 * @param[in] input column
 * @param[in] requested quantile in [0,1]
 * @param[out] result; type-erased result of same type as column;
 * @param[in] struct with additional info
 *
 * @returns GDF_SUCCESS upon successful compute, otherwise returns appropriate error code
 */
gdf_error gdf_quantile_approx(gdf_column* col_in,
                              double q,
                              void* t_erased_res,
                              gdf_context* ctxt);

/** 
 * @brief Replace elements from `col` according to the mapping `old_values` to
 *        `new_values`, that is, replace all `old_values[i]` present in `col` 
 *        with `new_values[i]`.
 * 
 * @param[in,out] col gdf_column with the data to be modified
 * @param[in] old_values gdf_column with the old values to be replaced
 * @param[in] new_values gdf_column with the new values
 * 
 * @returns GDF_SUCCESS upon successful completion
 *
 */
gdf_error gdf_find_and_replace_all(gdf_column*       col,
                                   const gdf_column* old_values,
                                   const gdf_column* new_values);

/** 
 * @brief Sorts an array of gdf_column.
 * 
 * @param[in] input_columns Array of gdf_columns
 * @param[in] asc_desc Device array of sort order types for each column
 *                     (0 is ascending order and 1 is descending). If NULL
 *                     is provided defaults to ascending order for evey column.
 * @param[in] num_inputs # columns
 * @param[in] flag_nulls_are_smallest Flag to indicate if nulls are to be considered
 *                                    smaller than non-nulls or viceversa
 * @param[out] output_indices Pre-allocated gdf_column to be filled with sorted
 *                            indices
 * 
 * @returns GDF_SUCCESS upon successful completion
 */
gdf_error gdf_order_by(gdf_column** input_columns,
                       int8_t*      asc_desc,
                       size_t       num_inputs,
                       gdf_column*  output_indices,
                       int          flag_nulls_are_smallest);

/**
 * @brief Replaces all null values in a column with either a specific value or corresponding values of another column
 *
 * This function is a binary function. It will take in two gdf_columns.

 * The first one is expected to be a regular gdf_column, the second one
 * has to be a column of the same type as the first, and it has to be of
 * size one or of the same size as the other column.
 * 
 * case 1: If the second column contains only one value, then this funciton will
 * replace all nulls in the first column with the value in the second
 * column.
 *  
 * case 2: If the second column is of the same size as the first, then the function will
 * replace all nulls of the first column with the corresponding elemetns of the
 * second column
 * 
 * @param[in,out] col_out A gdf_column that is the output of this function with null values replaced
 * @param[in] col_in A gdf_column that is of size 1 or same size as col_out, contains value / values to be placed in col_out
 * 
 * @returns GDF_SUCCESS upon successful completion
 */
gdf_error gdf_replace_nulls(gdf_column*       col_out,
                                   const gdf_column* col_in);
/**
 * @brief Finds the indices of the bins in which each value of the column
 * belongs.
 *
 * For `x` in `col`, if `right == false` this function finds
 * `i` such that `bins[i-1] <= x < bins[i]`. If `right == true`, it will find `i`
 * such that `bins[i - 1] < x <= bins[i]`. Finally, if `x < bins[0]` or
 * `x > bins[num_bins - 1]`, it sets the index to `0` or `num_bins`, respectively.
 *
 * NOTE: This function does not handle null values and will return an error if `col`
 * or `bins` contain any.
 *
 * @param[in] col gdf_column with the values to be binned
 * @param[in] bins gdf_column of ascending bin boundaries
 * @param[in] right Whether the intervals should include the left or right bin edge
 * @param[out] out_indices Output device array of same size as `col`
 * to be filled with bin indices
 *
 * @returns GDF_SUCCESS upon successful completion, otherwise an
 *         appropriate error code.
 */
gdf_error gdf_digitize(gdf_column* col,
                       gdf_column* bins,   // same type as col
                       bool right,
                       gdf_index_type out_indices[]);
