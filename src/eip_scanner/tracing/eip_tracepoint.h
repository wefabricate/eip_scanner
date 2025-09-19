#ifndef LTTNG_ENABLED

#ifndef _EIP_TRACEPOINT_H
#define _EIP_TRACEPOINT_H
#define lttng_ust_tracepoint(...)
#endif /* _EIP_TRACEPOINT_H */

#else

#undef LTTNG_UST_TRACEPOINT_PROVIDER
#define LTTNG_UST_TRACEPOINT_PROVIDER eip_scanner

#undef LTTNG_UST_TRACEPOINT_INCLUDE
#define LTTNG_UST_TRACEPOINT_INCLUDE "./eip_scanner/tracing/eip_tracepoint.h"

#if !defined(_EIP_TRACEPOINT_H) || defined(LTTNG_UST_TRACEPOINT_HEADER_MULTI_READ)
#define _EIP_TRACEPOINT_H

#include <lttng/tracepoint.h>

LTTNG_UST_TRACEPOINT_EVENT(
    eip_scanner,
    span_start,
    LTTNG_UST_TP_ARGS(
        const char*, span_name,
        int, span_id
    ),
    LTTNG_UST_TP_FIELDS(
        lttng_ust_field_string(span_name_field, span_name)
        lttng_ust_field_integer(int, span_id_field, span_id)
    )
)

LTTNG_UST_TRACEPOINT_EVENT(
    eip_scanner,
    span_stop,
    LTTNG_UST_TP_ARGS(
        int, span_id
    ),
    LTTNG_UST_TP_FIELDS(
        lttng_ust_field_integer(int, span_id_field, span_id)
    )
)

LTTNG_UST_TRACEPOINT_EVENT(
    eip_scanner,
    transmission_event,
    LTTNG_UST_TP_ARGS(
        const char*, event_name,
        unsigned long, seq_id,
        unsigned long, connection_id
    ),
    LTTNG_UST_TP_FIELDS(
        lttng_ust_field_string(event_name_field, event_name)
        lttng_ust_field_integer(unsigned long, seq_id_field, seq_id)
        lttng_ust_field_integer(unsigned long, connection_id_field, connection_id)
    )
)

LTTNG_UST_TRACEPOINT_EVENT(
    eip_scanner,
    new_connection,
    LTTNG_UST_TP_ARGS(
        unsigned int, serial_number,
        unsigned int, vendor_id,
        uint8_t*, connection_path,
        size_t, connection_path_length,
        unsigned long, o2t_connection_id,
        unsigned long, t2o_connection_id
    ),
    LTTNG_UST_TP_FIELDS(
        lttng_ust_field_integer(unsigned int, serial_number_field, serial_number)
        lttng_ust_field_integer(unsigned int, vendor_id_field, vendor_id)
        lttng_ust_field_sequence_hex(uint8_t, connection_path_field, connection_path, size_t, connection_path_length)
        lttng_ust_field_integer(unsigned long, o2t_connection_id_field, o2t_connection_id)
        lttng_ust_field_integer(unsigned long, t2o_connection_id_field, t2o_connection_id)
    )
)

LTTNG_UST_TRACEPOINT_EVENT(
    eip_scanner,
    close_connection,
    LTTNG_UST_TP_ARGS(
        unsigned long, o2t_connection_id,
        unsigned long, t2o_connection_id
    ),
    LTTNG_UST_TP_FIELDS(
        lttng_ust_field_integer(unsigned long, o2t_connection_id_field, o2t_connection_id)
        lttng_ust_field_integer(unsigned long, t2o_connection_id_field, t2o_connection_id)
    )
)

#endif /* _EIP_TRACEPOINT_H */

#include <lttng/tracepoint-event.h>
#endif /* __linux__ */