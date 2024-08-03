project('orb', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++23'])

#libraries
boost_preprocessor_inc = include_directories('lib/include')
src_inc = include_directories('src')

# Add all source files
sources = files()
subdir('src')

executable('orb',
           'src/main.cpp',
           sources,
           install : true,
           include_directories : [boost_preprocessor_inc, src_inc])

# Run tests
subdir('tests')
