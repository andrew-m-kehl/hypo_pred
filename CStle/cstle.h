#ifndef CSTLE_H
#define CSTLE_H

#ifdef __cplusplus
    extern "C" {
#endif

#ifdef _WIN32
    #if defined(CSTLE_COMPILE_DLL)
        #define CSTLE_API __declspec(dllexport)
    #elif defined(CSTLE_USE_DLL)    
        #define CSTLE_API __declspec(dllimport)
    #else
        #define CSTLE_API
    #endif
#else
    #if defined(CSTLE_COMPILE_DLL)
        #define CSTLE_API __attribute__ ((visibility ("default")))
    #else
      #define CSTLE_API
    #endif
#endif

#include <stdio.h>

CSTLE_API const char *stle_version(void);

CSTLE_API const char *stle_c_api_version(void);

typedef struct stle_pcseries stle_pcseries;

CSTLE_API stle_pcseries *stle_make_pcseries(void);

CSTLE_API int stle_pcseries_add(stle_pcseries *series, double endTime, double value);

CSTLE_API double stle_pcseries_value0(const stle_pcseries *series);

CSTLE_API double stle_pcseries_value(const stle_pcseries *series, int i);

CSTLE_API double stle_pcseries_start_time(const stle_pcseries* series, int i);

CSTLE_API int stle_pcseries_size(const stle_pcseries *series);

CSTLE_API int stle_pcseries_get_eps_separation_size(const stle_pcseries *series, double epsilon);

CSTLE_API int stle_pcseries_equal(const stle_pcseries *series1, const stle_pcseries *series2);

CSTLE_API void stle_delete_pcseries(const stle_pcseries *series);

typedef struct stle_pcsignal stle_pcsignal;

CSTLE_API int stle_pcsignal_size(const stle_pcsignal *signal);

CSTLE_API const stle_pcseries *stle_pcsignal_at(const stle_pcsignal *signal, int i);

// TODO: More PCSignal methods/functions.

CSTLE_API void stle_delete_pcsignal(const stle_pcsignal *signal);

// These should match Stl::ReadSignalFlags.
#define STLE_READ_SIGNAL_TIME_FIRST 0x0
#define STLE_READ_SIGNAL_TIME_LAST 0x1
#define STLE_READ_SIGNAL_START_POINTS 0x0
#define STLE_READ_SIGNAL_END_POINTS 0x2
#define STLE_READ_SIGNAL_RIGHT_OPEN 0x0
#define STLE_READ_SIGNAL_RIGHT_CLOSED 0x4
#define STLE_READ_SIGNAL_DEFAULT 0x0

CSTLE_API stle_pcsignal *stle_read_pcsignal_array(const double *data, int sample_count, int field_count, int by_row, int flags);

CSTLE_API stle_pcsignal *stle_read_pcsignal_array2(double const *const *data, int sample_count, int field_count, int flags);

CSTLE_API stle_pcsignal *stle_read_pcsignal_csv_file(FILE *file, int flags);

CSTLE_API stle_pcsignal *stle_read_pcsignal_csv_fname(const char *fileName, int flags);

typedef struct stle_exprset stle_exprset;

CSTLE_API stle_exprset *stle_make_exprset(void);

CSTLE_API void stle_delete_exprset(const stle_exprset *exprset);

// TODO: Need two concepts, a holding reference (like unique_ptr<ExprPtr>) and a non-holding (like ExprPtr&).
typedef struct stle_expr stle_expr;

typedef struct stle_expr_impl stle_expr_impl;

CSTLE_API const struct stle_expr_impl *stle_get_expr_impl(const stle_expr *expr);

// TODO: Version that accepts length and/or pointer past the end (now the implementation calls strlen on str).
CSTLE_API const stle_expr *stle_parse_sexpr_str(stle_exprset *exprset, const char *str, int *pos);

CSTLE_API const stle_expr *stle_parse_sexpr_file(stle_exprset *exprset, FILE *file);

CSTLE_API void stle_unref_expr(const stle_expr *expr);

typedef struct stle_signalvars stle_signalvars;

// TODO: Creating non-xn varmap.

CSTLE_API stle_signalvars *stle_make_signalvars_xn(int n);

CSTLE_API void stle_delete_signalvars(const stle_signalvars *signalvars);

typedef struct stle_pcmonitor stle_pcmonitor;

CSTLE_API stle_pcmonitor *stle_make_pcmonitor(stle_pcsignal *signal, const stle_signalvars *signalvars, stle_exprset *exprset);

CSTLE_API const stle_pcseries *stle_pcmonitor_make_output(stle_pcmonitor *monitor, const stle_expr *expr, int rewrite, const stle_expr **rewritten);

CSTLE_API void stle_delete_pcmonitor(const stle_pcmonitor *monitor);

// Aliases for the functions without 'offline' in the name.
typedef struct stle_pcmonitor stle_offlinepcmonitor;

CSTLE_API stle_offlinepcmonitor *stle_make_offlinepcmonitor(stle_pcsignal *signal, const stle_signalvars *signalvars, stle_exprset *exprset);

CSTLE_API const stle_pcseries *stle_offlinepcmonitor_make_output(stle_offlinepcmonitor *monitor, const stle_expr *expr, int rewrite, const stle_expr **rewritten);

CSTLE_API void stle_delete_offlinepcmonitor(const stle_offlinepcmonitor *monitor);

typedef struct stle_pcrobmonitor stle_pcrobmonitor;

CSTLE_API stle_pcrobmonitor *stle_make_pcrobmonitor(stle_pcsignal *signal, const stle_signalvars *signalvars, stle_exprset *exprset);

CSTLE_API const stle_pcseries *stle_pcrobmonitor_make_output(stle_pcrobmonitor *monitor, const stle_expr *expr, int rewrite, const stle_expr **rewritten);

CSTLE_API void stle_delete_pcrobmonitor(const stle_pcrobmonitor *monitor);

#ifdef __cplusplus
    }
#endif

#endif
