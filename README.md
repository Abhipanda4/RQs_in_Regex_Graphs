1. Each node is represented by a set of 4 attrbutes:
    -> Year of birth(integer)
    -> Name(string)
    -> Gender(string)
    -> UID(integer)

2. A node's attribute can be specified by an ops string like:
    -- <=1990==Panda!=F<=180
    -> The possible ops for strings are: == & !=
    -> The possible ops for integers are: ==, >, >=, <, <=, !=
    -> In case, any attribute is not cared about, it is specified by __(double underscore)
