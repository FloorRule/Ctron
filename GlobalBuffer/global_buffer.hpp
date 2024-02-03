/**
 * @file global_buffer.hpp
 * @author Avihai Shalom <avihais12344@gmail.com>
 * @brief A class to store all the commands to the llvm.
 * @version 0.1
 * @date 2021-02-01
 * 
 * @copyright Copyright (c) 2021
 * If we have license, view it. If not, notify the author.
 * 
 */

#ifndef __GLOBAL_BUFFER_HPP__
#define __GLOBAL_BUFFER_HPP__ 1

#include <list>         //std::list.
#include <string>       //std::string.
#include <cstddef>
#include <iostream>      //std::size_t
#include <vector>

using std::string;

///The namespace that has all the Ctron project componenets.
namespace Ctron{

    /**
     * @brief A class to store all the commands to the llvm.
     * It has commands stream to store all the commands and
     * global variables declerations stream to store all the global variables declerations.
     */
    class global_buffer{
    
    public:
    ///A container to store the commands themselves.
    typedef std::list<std::string> command_container_t;
    ///A type to count the labels beign used.
    typedef unsigned long long int label_counter_t;
    
    /**
     * @brief Construct a new global buffer object.
     * Default constractor.
     * There will be no commands and the number of labels will be 0.
     */
    global_buffer();
    
    /**
     * @brief Construct a new global buffer object from another global buffer object to copy.
     * @param other the other global buffer object to copy to this global buffer object with.
     */
    global_buffer(const global_buffer& other);

    /**
     * @brief Copy other global buffer object to this global buffer object.
     * @param other The other global buffer object to copy.
     * @return this object.
     */
    global_buffer& operator=(const global_buffer& other);

    /**
     * @brief Get the commands as string.
     * @return The commands as string.
     */
    std::string get_commands() const;

    /**
     * @brief Get the global variables declerations as string.
     * @return global variables declerations as string.
     */
    std::string get_global_vars_declerations() const;

    /**
     * @brief Get the commands container.
     * @return The commands container.
     */
    const command_container_t& get_commands_container() const noexcept;

    /**
     * @brief Get the global variables declerations container.
     * @return global variables declerations container.
     */
    const command_container_t& get_global_vars_declerations_container() const noexcept;
    
    /**
     * @brief This function inserts a command to the commands stream.
     * WARNING: The command should be without newline.
     * @param command The command to insert to the commands stream. The command should be without newline.
     * @warning The command should be without newline.
     */
    int insert_command(const std::string& command);
	
	void replaceLine(std::vector<string> replacedLine, std::vector<int> posL);
	
	void replaceCallLine(string line, int pos);
	
    /**
     * @brief This function inserts a command to the commands stream.
     * WARNING: The command should be without newline.
     * @param str The command to insert to the commands stream (as C str). The command should be without newline.
     * @warning The command should be without newline.
     */
    int insert_command(const char* str);
	
    /**
     * @brief This function inserts a command to the commands stream.
     * It copies the command from the buffer `str`.
     * WARNING: The command should be without newline.
     * @param str The buffer to copy the command from.
     * @param size The number of chars to copy from the buffer.
     * @warning The command should be without newline.
     */
    int insert_command(const char* str, std::size_t size);

    /**
     * @brief This function inserts a global variable decleration to the global variable declerations stream.
     * WARNING: The global variable decleration should be without newline.
     * @param command The global variable decleration to insert to the global variable declerations stream.
     * The global variable decleration should be without newline.
     * @warning The global variable decleration should be without newline.
     */
    void insert_global_variable_decleration(const std::string& command);

    /**
     * @brief This function inserts a global variable decleration to the global variable declerations stream.
     * WARNING: The global variable decleration should be without newline.
     * @param str The global variable decleration to insert to the global variable declerations stream (as C str).
     * The global variable decleration should be without newline.
     * @warning The global variable decleration should be without newline.
     */
    void insert_global_variable_decleration(const char* str);
	

    /**
     * @brief This function inserts a global variable decleration to the global variable declerations stream.
     * WARNING: The global variable decleration should be without newline.
     * It copies the global variable decleration from the buffer `str`.
     * @param str The buffer to copy the global variable decleration from.
     * @param size The number of chars to copy from the buffer.
     * @warning The global variable decleration should be without newline.
     */
    void insert_global_variable_decleration(const char* str, std::size_t size);

	void write_global_container_to_file(FILE* file);
	void write_container_to_file(FILE* file);

    /**
     * @brief This function generates a label for a variable at the global buffer.
     * @return The label of the variable.
     */
    std::string generate_label();

    private:
    
    /**
     * @brief Container to store the regular commands.
     */
    command_container_t _commands;

    /**
     * @brief Container to store the global vars decleration.
     */
    command_container_t _global_vars_declerations;

    /**
     * @brief The number of labels this buffer generated.
     * @see global_buffer::generate_label()
     */
    label_counter_t _num_labels;
    }; //class global_buffer.

} //namespace Ctron.

#endif // __GLOBAL_BUFFER_HPP__