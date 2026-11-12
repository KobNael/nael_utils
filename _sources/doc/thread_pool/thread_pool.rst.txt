.. _thread_pool:

Thread Pool
=========================

A job runs with the :ref:`log session <log_session>` of the thread which enqueued it:
its logs go to the directory and verbosity of that session, not to the process wide loggers.

.. doxygenclass:: thread::Pool
    :members:

