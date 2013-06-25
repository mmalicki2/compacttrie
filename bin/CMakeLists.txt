include_directories (${TRIE_SOURCE_DIR}/libcompacttrie) 
include_directories (${TRIE_SOURCE_DIR}/libmeasure_resource) 

link_directories (${TRIE_BINARY_DIR}/libcompacttrie) 
link_directories (${TRIE_BINARY_DIR}/libmeasure_resource) 

add_executable (ChunkManager_test ChunkManager_test.cpp) 
add_executable (Performance_test Performance_test.cpp) 
add_executable (CompactTrie_test CompactTrie_test.cpp) 
add_executable (SimpleTrie_test SimpleTrie_test.cpp) 
add_executable (Bit_test Bit_test.cpp) 
add_executable (cmp_test cmp_test.cpp) 

#target_link_libraries (ChunkStorage_test Resource) 
target_link_libraries (Performance_test Resource) 
target_link_libraries (cmp_test Resource) 
