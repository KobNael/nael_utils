.. _log_utils:

Log utilities
===================

Here you can find some tools for logging purpose :
    - The verbosity is defined with the enum :ref:`log_level`
    - Two loggers are available :
        - A basic :ref:`log_file`
        - :ref:`tee_logger`, allowing to simultaneously print in the terminal and in a file
    - A :ref:`tee_logger_manager` is also provided
    - :ref:`macros` to log while taking the verbosity into account

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
.. doxygenclass:: io::TeeLogger
    :members:


.. _tee_logger_manager:

TeeLogger Manager
^^^^^^^^^^^^^^^^^^^^^

One can create, access and set the verbosity an instance of :ref:`tee_logger` using the following class :

.. doxygenclass:: io::LoggerManager
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

