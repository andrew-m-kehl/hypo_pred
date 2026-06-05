#include "cstle.h"

#include "Stle/PCSignal.hpp"
#include "Stle/ArrayPCSignalReader.hpp"
#include "Stle/CsvPCSignalReader.hpp"
#include "Stle/ExprSet.hpp"
#include "Stle/SExprParser.hpp"
#include "Stle/PCMonitor.hpp"
#include "Stle/PCRobMonitor.hpp"
#include "Stle/ExprPtr.hpp"
#include "Stle/Version.hpp"

#include "Stle/U/FileReader.hpp"
#include "Stle/U/CsvReader.hpp"
#include "Stle/U/SLexer.hpp"
#include "Stle/U/Array2DRef.hpp"
#include "Stle/U/Array2DTRef.hpp"
#include <Stle/PCSeriesCsvPrinter.hpp>

using namespace Stle;

const char *stle_version() {
    return stleVersion();
}

const char *stle_c_api_version() {
    return CSTLE_VERSION;
}

stle_pcseries *stle_make_pcseries() {
    stle_pcseries *series = nullptr;
    try {
        series = reinterpret_cast<stle_pcseries*>(new PCSeries());
    } catch (...) {        
    }
    return series;
}

int stle_pcseries_add(stle_pcseries *series, double endTime, double value) {
    try {
        reinterpret_cast<PCSeries*>(series)->add(Time(endTime), Space(value));    
        return 1;
    } catch (...) {        
    }
    return 0;    
}

double stle_pcseries_value0(const stle_pcseries *series) {
    return reinterpret_cast<const PCSeries*>(series)->at(0).value().value();   
}

double stle_pcseries_value(const stle_pcseries *series, int i) {
    return reinterpret_cast<const PCSeries*>(series)->at(i).value().value();
}

double stle_pcseries_start_time(const stle_pcseries* series, int i) {
    return reinterpret_cast<const PCSeries*>(series)->startTime(i).value();
}

int stle_pcseries_size(const stle_pcseries *series) {
    return reinterpret_cast<const PCSeries*>(series)->size();
}

int getNextTrueIndex(const PCSeries* access_series, int previousIndex){
    int startIndex = previousIndex + 1;
    int series_len = access_series->size();
    int i = 0;
    i = startIndex;
    while(i < series_len){
        if( (access_series->at(i)).value() == Double(1)){
            break;
        }
        i++;
    }
    return i;
}

void moveTimeNeedleByEpsilon(const PCSeries* access_series, Time epsilon, int &currentIndex, Time &startOfTime){
    int series_len = access_series->size();
    Time tBegin = access_series->startTime(currentIndex);

    if(startOfTime < tBegin){
        startOfTime = tBegin;
    }
    int i = currentIndex;
    while( i < series_len && (startOfTime + epsilon) >= (access_series->at(i)).endTime() ){
        i = getNextTrueIndex(access_series, i);
    }
    currentIndex = i;
    startOfTime += epsilon;
}

int stle_pcseries_get_eps_separation_size(const stle_pcseries *series, double epsilon) {
    const PCSeries* access_series = reinterpret_cast<const PCSeries*>(series);
    Time epsTime(epsilon);
    int series_len = access_series->size();
    if(series_len == 0){
        return 0;
    }
    Time startOfTime(0);
    int epsCoveringSize = 0;
    int currentIndex = getNextTrueIndex(access_series, -1); // initialization.
    while(currentIndex < series_len){
        moveTimeNeedleByEpsilon(access_series, epsTime, currentIndex, startOfTime);
        epsCoveringSize++;
    }
    return epsCoveringSize;
}

int stle_pcseries_equal(const stle_pcseries *series1, const stle_pcseries *series2) {
    return static_cast<int>(*reinterpret_cast<const PCSeries*>(series1) == *reinterpret_cast<const PCSeries*>(series2));
}

void stle_delete_pcseries(const stle_pcseries *series) {
    if (series)
        delete reinterpret_cast<const PCSeries*>(series) ;
}

int stle_pcsignal_size(const stle_pcsignal *signal) {
    return reinterpret_cast<const PCSignal*>(signal)->size();
}

const stle_pcseries *stle_pcsignal_at(const stle_pcsignal *signal, int i) {
    return reinterpret_cast<const stle_pcseries*>(&reinterpret_cast<const PCSignal*>(signal)->at(i));
}

void stle_delete_pcsignal(const stle_pcsignal *signal) {
    if (signal)
        delete reinterpret_cast<const PCSignal*>(signal);
}

stle_pcsignal *stle_read_pcsignal_array(const double *data, int sample_count, int field_count, int by_row, int flags) {
    stle_pcsignal *signal = nullptr;
    try {
        if (by_row)
            signal = reinterpret_cast<stle_pcsignal*>(new PCSignal(readPCSignalArray(U::makeArray2DRef(data, field_count), sample_count, field_count, static_cast<ReadSignalFlags>(flags))));
        else
            signal = reinterpret_cast<stle_pcsignal*>(new PCSignal(readPCSignalArray(U::makeArray2DTRef(data, sample_count), sample_count, field_count, static_cast<ReadSignalFlags>(flags))));
    } catch (...) {
    }
    return signal;
}

stle_pcsignal *stle_read_pcsignal_array2(double const *const *data, int sample_count, int field_count, int flags) {
    stle_pcsignal *signal = nullptr;
    try {
        signal = reinterpret_cast<stle_pcsignal*>(new PCSignal(readPCSignalArray(data, sample_count, field_count, static_cast<ReadSignalFlags>(flags))));
    } catch (...) {        
    }
    return signal;    
}

stle_pcsignal *stle_read_pcsignal_csv_file(FILE *file, int flags) {
    stle_pcsignal *signal = nullptr;
    try {
        U::CsvReader<U::FileReader<U::FileRef>> csvReader(U::OF_READER, U::OF_FILE, file);
        signal = reinterpret_cast<stle_pcsignal*>(new PCSignal(readPCSignalCsv(csvReader, static_cast<ReadSignalFlags>(flags))));
    } catch (...) { 
    }
    return signal;
}

stle_pcsignal *stle_read_pcsignal_csv_fname(const char *fileName, int flags) {
    stle_pcsignal *signal = nullptr;
    try {        
        U::CsvReader<U::FileReader<U::File>> csvReader(U::OF_READER, U::OF_FILE, fileName, "r");
        signal = reinterpret_cast<stle_pcsignal*>(new PCSignal(readPCSignalCsv(csvReader, static_cast<ReadSignalFlags>(flags))));
    } catch (...) { 
    }
    return signal;
}

stle_exprset *stle_make_exprset() {
    stle_exprset *exprset = nullptr;
    try {
        exprset = reinterpret_cast<stle_exprset*>(new ExprSet());
    } catch (...) {
    }
    return exprset;
}

void stle_delete_exprset(const stle_exprset *exprset) {
    if (exprset)    
        delete reinterpret_cast<const ExprSet*>(exprset);
}

const stle_expr_impl *stle_get_expr_impl(const stle_expr *expr) {
    return reinterpret_cast<const stle_expr_impl*>(reinterpret_cast<const ExprPtr*>(expr)->get());
}

const stle_expr *stle_parse_sexpr_str(stle_exprset *exprset, const char *str, int *pos) {
    const stle_expr *expr = nullptr;
    try {        
        expr = reinterpret_cast<const stle_expr*>(new ExprPtr(parseSExprStr(*reinterpret_cast<ExprSet*>(exprset), str, pos)));
    } catch (...) {
    }
    return expr;    
}

const stle_expr *stle_parse_sexpr_file(stle_exprset *exprset, FILE *file) {
    const stle_expr *expr = nullptr;
    try {
        SExprParser<U::SLexer<U::FileReader<U::FileRef>>> parser(*reinterpret_cast<ExprSet*>(exprset), U::OF_READER, U::OF_FILE, file);
        expr = reinterpret_cast<const stle_expr*>(new ExprPtr(parser.parse()));
    } catch (...) {
    }
    return expr;    
}

void stle_unref_expr(const stle_expr *expr) {
    if (expr)
        delete reinterpret_cast<const ExprPtr*>(expr);    
}

stle_signalvars *stle_make_signalvars_xn(int n) {
    stle_signalvars *signalvars = nullptr;
    try {
        signalvars = reinterpret_cast<stle_signalvars*>(new SignalVars(VARS_XN, n));
    } catch (...) {
    }    
    return signalvars;
}

void stle_delete_signalvars(const stle_signalvars *signalvars) {
    if (signalvars)
        delete reinterpret_cast<const SignalVars*>(signalvars);
}

stle_pcmonitor *stle_make_pcmonitor(stle_pcsignal *signal, const stle_signalvars *signalvars, stle_exprset *exprset) {
    stle_pcmonitor *monitor = nullptr;
    try {
        monitor = reinterpret_cast<stle_pcmonitor*>(
            new PCMonitor(
                *reinterpret_cast<PCSignal*>(signal),
                *reinterpret_cast<const SignalVars*>(signalvars),
                *reinterpret_cast<ExprSet*>(exprset)));
    } catch (...) {
    }
    return monitor;
}

const stle_pcseries *stle_pcmonitor_make_output(stle_pcmonitor *monitor, const stle_expr *expr, int rewrite, const stle_expr **rewritten) {
    const stle_pcseries *result = nullptr;
    try {
        ExprPtr rewrittenExpr;        
        result = reinterpret_cast<const stle_pcseries*>(
            &reinterpret_cast<PCMonitor*>(monitor)->makeOutput(
                *reinterpret_cast<const ExprPtr*>(expr),
                static_cast<bool>(rewrite),
                &rewrittenExpr));
        if (rewrite && rewritten) {            
            *rewritten = reinterpret_cast<const stle_expr*>(new ExprPtr(std::move(rewrittenExpr)));
        }
    } catch (...) {
    }
    return result;
}

void stle_delete_pcmonitor(const stle_pcmonitor *monitor) {
    if (monitor)
        delete reinterpret_cast<const PCMonitor*>(monitor);
}

stle_pcmonitor *stle_make_offlinepcmonitor(stle_pcsignal *signal, const stle_signalvars *signalvars, stle_exprset *exprset) {
    return stle_make_pcmonitor(signal, signalvars, exprset);
}

const stle_pcseries *stle_offlinepcmonitor_make_output(stle_offlinepcmonitor *monitor, const stle_expr *expr, int rewrite, const stle_expr **rewritten) {
    return stle_pcmonitor_make_output(monitor, expr, rewrite, rewritten);
}

void stle_delete_offlinepcmonitor(const stle_offlinepcmonitor *monitor) {
    return stle_delete_pcmonitor(monitor);
}

stle_pcrobmonitor *stle_make_pcrobmonitor(stle_pcsignal *signal, const stle_signalvars *signalvars, stle_exprset *exprset) {
    stle_pcrobmonitor *monitor = nullptr;
    try {
        monitor = reinterpret_cast<stle_pcrobmonitor*>(
            new PCRobMonitor(
                *reinterpret_cast<PCSignal*>(signal),
                *reinterpret_cast<const SignalVars*>(signalvars),
                *reinterpret_cast<ExprSet*>(exprset)));
    } catch (...) {
    }
    return monitor;
}

const stle_pcseries *stle_pcrobmonitor_make_output(stle_pcrobmonitor *monitor, const stle_expr *expr, int rewrite, const stle_expr **rewritten) {
    const stle_pcseries *result = nullptr;
    try {
        ExprPtr rewrittenExpr;        
        result = reinterpret_cast<const stle_pcseries*>(
            &reinterpret_cast<PCRobMonitor*>(monitor)->makeOutput(
                *reinterpret_cast<const ExprPtr*>(expr),
                static_cast<bool>(rewrite),
                &rewrittenExpr));
        if (rewrite && rewritten) {            
            *rewritten = reinterpret_cast<const stle_expr*>(new ExprPtr(std::move(rewrittenExpr)));
        }
    } catch (...) {
    }
    return result;
}

void stle_delete_pcrobmonitor(const stle_pcrobmonitor *monitor) {
    if (monitor)
        delete reinterpret_cast<const PCRobMonitor*>(monitor);
}
