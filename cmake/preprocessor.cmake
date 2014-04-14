###
# This software is provided 'as-is', without any express or implied
# warranty.  In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
# 
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.
#
# Note: Some of the libraries Totem EDK may link to may have additional
# requirements or restrictions.
#
###
# CMake system for Totem EDK was contributed by Jacob 'jacmoe' Moen
#
###

#-----------------------------
# DEFINE PATHS TO TOTEM FILES
#-----------------------------
macro(define_totem_paths package)
	set(TOTEM_HDRS ${TotemEDK_SOURCE_DIR}/include/Totem)
	set(TOTEM_HDRS_ADDONS ${TotemEDK_SOURCE_DIR}/include/Totem/Addons)
	set(TOTEM_CONFIG_HDRS ${TotemEDK_SOURCE_DIR}/depends/${package})
	
	message(STATUS "Path to Totem include/Totem/: ${TOTEM_HDRS}")
	message(STATUS "Path to Totem include/Totem/Addons/: ${TOTEM_HDRS_ADDONS}")
	message(STATUS "Path to ${package} configuration headers for Totem: ${TOTEM_CONFIG_HDRS}")
	message(STATUS " ")
endmacro(define_totem_paths)

#-------------------------------
# EXTRACT TOTEM FILES FROM DISK
#-------------------------------
macro(extract_files package)
	file(GLOB totem_hdr_files_abs_path ${TOTEM_HDRS}/*.h ${TOTEM_HDRS}/*.inl)
	file(GLOB totem_hdr_addon_files_abs_path ${TOTEM_HDRS_ADDONS}/*.h ${TOTEM_HDRS_ADDONS}/*.inl)
	file(GLOB totem_config_hdrs_abs_path ${TOTEM_CONFIG_HDRS}/*.h ${TOTEM_CONFIG_HDRS}/*.inl)
	message(STATUS "Header Files in include/Totem/ ${totem_hdr_files_abs_path}")
	message(STATUS "Header Files in include/Totem/Addons/ ${totem_hdr_addon_files_abs_path}")
	message(STATUS "${package} configuration headers for Totem in depends/${package}/ ${totem_config_hdrs_abs_path}")
	message(STATUS " ")
endmacro(extract_files)

#---------------------------------
# DEFINE OUTPUT DESTINATION PATHS
#---------------------------------
macro(define_output_paths package)
	set(PACKAGE_PATH_NAME "${package}")
	set(PACKAGE_OUTPUT_DIR ${TotemEDK_SOURCE_DIR}/editions/${PACKAGE_PATH_NAME})
	set(PACKAGE_HDRS_DIR ${PACKAGE_OUTPUT_DIR}/include/Totem)
	set(PACKAGE_HDRS_ADDONS_DIR ${PACKAGE_OUTPUT_DIR}/include/Totem/Addons)
	set(PACKAGE_CONFIG_HDRS_DIR ${PACKAGE_OUTPUT_DIR}/include/Totem)

	message(STATUS "Output Path to Totem ${package} include/Totem/: ${PACKAGE_HDRS_DIR}")
	message(STATUS "Output Path to Totem ${package} include/Totem/Addons/: ${PACKAGE_HDRS_ADDONS_DIR}")
	message(STATUS "Output Path to Totem ${package} include/Totem/: ${PACKAGE_CONFIG_HDRS_DIR}")
	message(STATUS " ")
endmacro(define_output_paths)

#--------------------------------------------------------
# SPLIT EXTRACTED TOTEM FILE PATH STRINGS INTO FILENAMES
#--------------------------------------------------------
macro(split_into_filenames package)
	set(totem_hdr_files)
	set(totem_hdr_ignore_files)
	set(totem_hdr_addon_files)
	set(totem_config_hdr_files)

	message(STATUS "include/Totem/ filenames:")
	FOREACH(f ${totem_hdr_files_abs_path})
		FILE(RELATIVE_PATH folder ${TOTEM_HDRS} ${f})
		get_filename_component(filename ${folder} NAME)
		
		STRING(COMPARE EQUAL ${filename} "mainpage.h" string_compare_result)
		
		if(${string_compare_result})
			set(totem_hdr_ignore_files ${totem_hdr_ignore_files} ${f})
			message(STATUS "THROW AWAY ${filename}")
		endif()
		
		if(NOT ${string_compare_result})
			set(totem_hdr_files ${totem_hdr_files}  ${filename})
			message(STATUS "- ${filename}")
		endif()
	ENDFOREACH(f ${ARGN})
	message(STATUS " ")
	message(STATUS "include/Totem/Addons/ filenames:")
	FOREACH(f ${totem_hdr_addon_files_abs_path})
		FILE(RELATIVE_PATH folder ${TOTEM_HDRS_ADDONS} ${f})
		get_filename_component(filename ${folder} NAME)
		set(totem_hdr_addon_files ${totem_hdr_addon_files} ${filename})
		message(STATUS "- ${filename}")
	ENDFOREACH(f ${ARGN})
	message(STATUS " ")
	message(STATUS "include/Totem/config.h:")
	FOREACH(f ${totem_config_hdrs_abs_path})
		FILE(RELATIVE_PATH folder ${TOTEM_HDRS} ${f})
		get_filename_component(filename ${folder} NAME)
		set(totem_config_hdr_files ${totem_config_hdr_files} ${filename})
		message(STATUS "- ${filename}")
	ENDFOREACH(f ${ARGN})
	message(STATUS " ")
endmacro(split_into_filenames)

#---------------------------------
# DEFINE ABSOLUTE PATH FOR OUTPUT
#---------------------------------
macro(define_output_abs_paths package)
	set(package_hdr_files)	
	set(package_hdr_addon_files)
	set(package_config_hdr_files)
	
	foreach(f ${totem_hdr_files})
		set(package_hdr_files ${package_hdr_files} ${PACKAGE_HDRS_DIR}/${f})
	endforeach()
	foreach(f ${totem_hdr_addon_files})
		set(package_hdr_addon_files ${package_hdr_addon_files} ${PACKAGE_HDRS_ADDONS_DIR}/${f})
	endforeach()
	foreach(f ${totem_config_hdr_files})
		set(package_config_hdr_files ${package_config_hdr_files} ${PACKAGE_CONFIG_HDRS_DIR}/${f})
	endforeach()

	message(STATUS "Header Files to editions/${package}/include/Totem/ ${package_hdr_files}")
	message(STATUS "Header Files to editions/${package}/include/Totem/Addons/ ${package_hdr_addon_files}")
	message(STATUS "${package} Configuration Headers to editions/${package}/include/Totem/ ${package_config_hdr_files}")
	message(STATUS " ")
endmacro(define_output_abs_paths)

#---------------------------------
# COPY TOTEM FILES TO OUTPUT DIR
#---------------------------------
macro(copy_totem_to_output package)
	set(list_count)
	set(list_index 0)
	list(LENGTH totem_hdr_files_abs_path list_count)
	foreach(input ${totem_hdr_files_abs_path})
		set(ignore_index 0)
		foreach(ignore ${totem_hdr_ignore_files})
			STRING(COMPARE EQUAL ${input} ${ignore} ignore_this_index)
			if(${ignore_this_index})
				set(ignore_index 1)
			endif(${ignore_this_index})
		endforeach(ignore ${totem_hdr_ignore_files})
		
		if(NOT ${ignore_index})
			list(GET package_hdr_files ${list_index} output)
			message(STATUS "Evaluating index ${list_index}/${list_count}: ${output}")
			MATH(EXPR list_index "${list_index}+1")
			
			configure_file(${input} ${output} COPYONLY)
		endif(NOT ${ignore_index})
	endforeach(input ${totem_hdr_files_abs_path})
	message(STATUS " ")
	
	
	set(list_count)
	set(list_index 0)
	list(LENGTH package_hdr_addon_files list_count)
	foreach(input ${totem_hdr_addon_files_abs_path})
		list(GET package_hdr_addon_files ${list_index} output)
		message(STATUS "Evaluating index ${list_index}/${list_count}: ${output}")
		MATH(EXPR list_index "${list_index}+1")
		
		configure_file(${input} ${output} COPYONLY)
	endforeach(input ${totem_hdr_addon_files_abs_path})
	message(STATUS " ")

	
	set(list_count)
	set(list_index 0)
	list(LENGTH package_config_hdr_files list_count)
	foreach(input ${totem_config_hdrs_abs_path})
		list(GET package_config_hdr_files ${list_index} output)
		message(STATUS "Evaluating index ${list_index}/${list_count}: ${output}")
		MATH(EXPR list_index "${list_index}+1")
		
		configure_file(${input} ${output} COPYONLY)
	endforeach(input ${totem_config_hdrs_abs_path})
	message(STATUS "----------------------------------------------")
	
	set(output_files ${package_hdr_files} ${package_hdr_addon_files})
endmacro(copy_totem_to_output)

#-------------------------------
# RUN FIND/REPLACE ON ALL FILES
#-------------------------------
macro(run_find_replace_on_output_files)
	foreach(output ${output_files})
		#Read the file
		FILE(READ "${output}" contents)
		
		#Replace all strings matching find with replace
		set(list_count)
		set(list_index 0)
		list(LENGTH list_replace list_count)
		foreach(find ${list_find})
			list(GET list_replace ${list_index} replace)
			message(STATUS "${list_index}/${list_count}: ${output}: ${find} -> ${replace}")
			MATH(EXPR list_index "${list_index}+1")
			STRING(REPLACE "${find}" "${replace}" contents "${contents}")
		endforeach(find ${list_find})
		
		#Replace all strings matching find with replace using regex
		set(list_count)
		set(list_index 0)
		list(LENGTH list_replace_regex list_count)
		foreach(find ${list_find_regex})
			list(GET list_replace_regex ${list_index} replace)
			message(STATUS "${list_index}/${list_count}: ${output}: ${find} -> ${replace}")
			MATH(EXPR list_index "${list_index}+1")
			STRING(REGEX REPLACE "${find}" "${replace}" contents "${contents}")
		endforeach(find ${list_find_regex})
		
		STRING(REPLACE ";" "\\;" contents "${contents}")
		
		#Overwrite the file with new contents
		FILE(WRITE ${output} ${contents})
	endforeach(output ${output_files})
endmacro(run_find_replace_on_output_files)

####################################
#----------------------------------#
# EMPTY THE PREPROCESSOR PAIR LIST #
#----------------------------------#
####################################
macro(empty_preprocess_pair_list)
	set(list_find)
	set(list_find_regex)
	set(list_replace)
	set(list_replace_regex)
endmacro(empty_preprocess_pair_list)

#########################################
#---------------------------------------#
# ADD FIND/REPLACE PAIR TO PREPROCESSOR #
#---------------------------------------#
#########################################
macro(add_preprocess_pair find replace)
	set(list_find 	 ${list_find} 	 ${find})
	set(list_replace ${list_replace} ${replace})
endmacro(add_preprocess_pair)

###############################################
#---------------------------------------------#
# ADD FIND/REPLACE REGEX PAIR TO PREPROCESSOR #
#---------------------------------------------#
###############################################
macro(add_preprocess_pair_regex find replace)
	set(list_find_regex 	${list_find_regex} 	  ${find})
	set(list_replace_regex 	${list_replace_regex} ${replace})
endmacro(add_preprocess_pair_regex)

############################
#--------------------------#
# PREPROCESS MAIN FUNCTION #
#--------------------------#
############################
macro(preprocess_totem package)
	message(STATUS "Preprocessing Totem EDK for ${package}")
	#---------------------------------------------------------------------
	define_totem_paths(${package})
	extract_files(${package})
	define_output_paths(${package})
	split_into_filenames(${package})
	define_output_abs_paths(${package})
	copy_totem_to_output(${package})
	run_find_replace_on_output_files()
	#---------------------------------------------------------------------
	message(STATUS "Preprocessing Totem EDK for ${package} is FINISHED!")
	message(STATUS "See editions/${package}/ for list of all files.")
endmacro(preprocess_totem)