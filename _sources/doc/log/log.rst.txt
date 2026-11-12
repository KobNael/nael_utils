.. _log_utils:

Log utilities
===================

Here you can find some tools for logging purpose :
    - The verbosity is defined with the enum :ref:`log_level`
    - Two loggers are available :
        - A basic :ref:`log_file`
        - :ref:`tee_logger`, allowing to simultaneously print in the terminal and in a file
    - A :ref:`logger_manager` is also provided
    - A :ref:`log_session` gives a thread its own loggers and settings (directory, verbosity, console), eg. for one run
    - :ref:`macros` to log while taking the verbosity into account

A log file is created (and truncated) with its logger, unless the logger is created at level ``OFF``:
any file left by a previous use is then removed, and the file is only created on the first write
(eg. after the level is raised). A logger which never writes creates no file.

------------------


.. _log_level:

Log level
------------------

The verbosity of the log can be set using

.. doxygenenum:: LogLevel



------------------


.. _loggers:

Loggers
------------------

.. _log_file:

Log file
^^^^^^^^^^^^^^^

A logfile can be managed through

.. doxygenclass:: io::FileLogger
    :members:

.. _tee_logger:

TeeLogger
^^^^^^^^^^^^^^^
The echo to the standard output can be disabled (``console = false``), eg. when the library runs inside
another process (a JVM) whose standard output must stay clean.

.. doxygenclass:: io::TeeLogger
    :members:


.. _logger_manager:

Logger Manager
^^^^^^^^^^^^^^^^^^^^^

One can create, access and set the verbosity an instance of :ref:`tee_logger` or :ref:`log_file` using the following class :

.. doxygenclass:: io::LoggerManager
    :members:

The loggers and settings of the manager are stored in a :ref:`log_state`: the process wide one,
or the one of the :ref:`log_session` alive on the calling thread.

.. _log_session:

Log session
^^^^^^^^^^^^^^^^^^^^^

Without session, the :ref:`logger_manager` uses a process wide state: the log files are written in the current
directory, and every thread shares the same loggers.

A ``LogSession`` gives the current thread its own state for its lifetime: log directory (created if needed),
default verbosity, default log file name and console echo. Every call of the :ref:`logger_manager` and every
:ref:`macro <macros>` made on this thread uses it; its log files are closed when it ends, and the previous
state of the thread is restored (sessions may be nested).

Sessions on different threads are independent, so several runs may log concurrently.
As the log files are named after the loggers, give each run its own directory.

.. code-block:: cpp

    void run(std::filesystem::path const &log_dir)
    {
        io::LogSession session(log_dir, io::LogLevel::INFO, "my_run", false);
        INFOLOG << "written to <log_dir>/my_run.log, not to the console" << std::endl;
        INFO(io::LoggerManager::GetFileLogger("detail")) << "written to <log_dir>/detail.log" << std::endl;
    } // files closed, the thread is back to the process wide loggers

The jobs of a :ref:`thread pool <thread_pool>` run with the session of the thread which enqueued them.
Another thread joins a session with a ``LogSessionScope``:

.. code-block:: cpp

    io::LogState *session = io::LoggerManager::CurrentSession();
    std::thread worker([session]() {
        io::LogSessionScope scope(session);
        INFOLOG << "logged in the session of the parent thread" << std::endl;
    });

Creating, getting and clearing the loggers of a state is thread safe (the threads of a pool share their state).
Writing to one logger from several threads at once is not: use one logger per thread or job.

.. warning::
    A thread started during a session without ``LogSessionScope`` uses the process wide state.
    The session must outlive the threads using it.

.. doxygenclass:: io::LogSession
    :members:

.. doxygenclass:: io::LogSessionScope
    :members:

.. _log_state:

Log state
^^^^^^^^^^^^^^^

.. doxygenstruct:: io::LogState
    :members:

.. _abstract_logger:

Logger
^^^^^^^^^^^^^^^
Both the :ref:`log_file` and the :ref:`tee_logger` inherits from the following class:

.. doxygenclass:: io::Logger
    :members:


--------------------------------------

.. _macros:

Macros
--------

.. _tee_logger_macros:

Main macros for :ref:`tee_logger` manipulation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygendefine:: ERORLOG
.. doxygendefine:: WARNLOG
.. doxygendefine:: INFOLOG
.. doxygendefine:: DBUGLOG
.. doxygendefine:: ERORLOG_RANGE
.. doxygendefine:: WARNLOG_RANGE
.. doxygendefine:: INFOLOG_RANGE
.. doxygendefine:: DBUGLOG_RANGE


.. _logger_macros:

Main macros for :ref:`log_file` manipulation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygendefine:: EROR
.. doxygendefine:: WARN
.. doxygendefine:: INFO
.. doxygendefine:: DBUG
.. doxygendefine:: EROR_RANGE
.. doxygendefine:: WARN_RANGE
.. doxygendefine:: INFO_RANGE
.. doxygendefine:: DBUG_RANGE


Full reference
----------------

.. doxygenfile:: log.hh
