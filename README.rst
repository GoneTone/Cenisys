Cenisys
=======

Canonical source
----------------

The source of Cenisys is maintained on GitLab.com_. Currently Cenisys is mirrored on GitHub for convenience.
You should make contributions on GitLab if possible.

.. _GitLab.com: https://gitlab.com/itxtech/cenisys

Ultra fast Minecraft server written in C++
------------------------------------------

Cenisys is written in C++ and has the following feature:

- Asynchronous I/O with Boost.Asio
- Utilizing the latest C++ standard to provide easy-to-use API

Requirements
------------

Cenisys utilizes bleeding-edge features, and we uses some library in development branch:

- Boost 1.61 (currently in git)

Cenisys uses C++14. GCC and clang is supported. MSVC isn't recommended since it lacks new standard support.

License
-------

Cenisys is licensed under GPLv3. Plugins must be licensed under a compatible license.

Special Thanks
--------------

- Thanks to PocketMine team: Protocol is based on PocketMine-MP source
- Thanks to Bukkit: API is inspired by Bukkit
