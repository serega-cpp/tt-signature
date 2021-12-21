#ifndef __OPTIONS_HPP__
#define __OPTIONS_HPP__

#include <string>
#include <stdexcept>
#include <sstream>

#include <boost/program_options.hpp>

struct Options
{
    bool        help;
    std::string src_fname;
    std::string dst_fname;
    size_t      block_size;

    boost::program_options::options_description desc;

    Options(int argc, char *argv[])
        : desc("Options")
    {
        desc.add_options()
            ("help", "produce help message")
            ("if", boost::program_options::value<std::string>()->default_value(""), "set input file name")
            ("of", boost::program_options::value<std::string>()->default_value(""), "set output file name")
            ("block", boost::program_options::value<int>()->default_value(1024), "set block size [kB]");

        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);

        help = (argc <= 1) || (vm.count("help") > 0);
        src_fname = boost::any_cast<std::string>(vm["if"].value());
        dst_fname = boost::any_cast<std::string>(vm["of"].value());
        block_size = vm["block"].as<int>() * 1024; // kB -> bytes

        auto err = Error();
        if (!err.empty())
            throw std::runtime_error(err);
    }

    std::string Usage()
    {
        std::stringstream ss;
        ss << desc;

        return ss.str();
    }

    std::string Error()
    {
        if (help) {
            return std::string();
        }
        if (src_fname.empty()) {
            return std::string("input file name not specified");
        }
        if (dst_fname.empty()) {
            return std::string("output file name not specified");
        }
        if (block_size == 0) {
            return std::string("block size not specified");
        }
        return std::string();
    }
};

#endif // __OPTIONS_HPP__
