[
{ "config": {
    "component": "shakespeare"
} },

{ "cc_library": {
    "name": "file_tokenizer",
    "cc_headers": [ "file_tokenizer.h" ],
    "cc_sources": [ "file_tokenizer.cc" ],
    "dependencies": [ "//common/file:linereader",
                      "//common/log:log",
                      "//common/strings:strutil",
                      "//third_party/boost:boost"
    ]
} },

{ "cc_library": {
    "name": "word_freq_map",
    "cc_headers": [ "word_freq_map.h" ],
    "dependencies": [ "//common/util:stl" ]
} },

{ "cc_library": {
    "name": "clusterer",
    "cc_headers": [ "clusterer.h" ],
    "cc_sources": [ "clusterer.cc" ]
} },

{ "cc_binary": {
    "name": "freq_compare",
    "cc_sources": [ "freq_compare.cc" ],
    "dependencies": [ "//common/base:base",
                     "//common/file:fileutil",
                      ":clusterer",
                      ":file_tokenizer",
                      ":word_freq_map"
    ]
} },

{ "py_binary": {
    "name": "strip_html",
    "py_sources": [ "strip_html.py" ]
} }
]