#include "Stle/U/ArgU.hpp"
#include "Stle/U/FileReader.hpp"
#include "Stle/U/CsvReader.hpp"
#include "Stle/U/FilePrinter.hpp"
#include "Stle/U/CounterU.hpp"

#include "Stle/SExprParser.hpp"
#include "Stle/PCSignal.hpp"
#include "Stle/PCMonitor.hpp"
#include "Stle/PCRobMonitor.hpp"
#include "Stle/CsvPCSignalReader.hpp"
#include "Stle/PCRewriter.hpp"
#include "Stle/PCSeriesGnuplotPrinter.hpp"
#include "Stle/PCSeriesCsvPrinter.hpp"
#include "Stle/ExprPtr.hpp"

#include "Stle/Cmd/CmdRunner.hpp"
#include "Stle/Cmd/PCState.hpp"
#include "Stle/Cmd/CmdRunnerVisitor.hpp"

#include "Stle/Version.hpp"

// Generated during build in the build directory.
#include "Blobs.hpp"

#include <cstdio>
#include <sstream>

using namespace Stle;

#ifdef _WIN32

#include "Stle/U/WinU.hpp"

int u8Main(int argc, char **u8Argv);

int wmain(int argc, wchar_t **argv) {
    std::unique_ptr<char*, U::ArgvDelete> u8Argv = U::makeU8Argv(argc, argv);
    return u8Main(argc, u8Argv.get());
}

#else

int u8Main(int argc, char **u8Argv);

int main(int argc, char **argv) {
    return u8Main(argc, argv);
}

#endif

void mainCli(U::ArgReader &argReader);
void mainCmd(U::ArgReader &argReader);
void mainHelp(U::ArgReader &argReader);

const int COUNTER_ID_MONITOR_TIME = 0;
const int COUNTER_ID_REWRITE_TIME = 1;
const int COUNTER_ID_READ_SIGNAL_TIME = 2;
const int COUNTER_ID_READ_EXPR_TIME = 3;

enum OutSignalFormat {
    OUT_SIGNAL_CSV,
    OUT_SIGNAL_GNUPLOT,
    OUT_SIGNAL_DEBUG
};

int u8Main(int argc, char **argv) {
    try {
        U::ArgReader argReader(argc, argv);
        // Skipping the first argument (executable path).
        argReader.read();
        // Reading the first actial argument.
        argReader.read();        
        if (argReader.token() == U::ARG_VALUE && !strcmp(argReader.str(), "cmd"))
            mainCmd(argReader);        
        else if (argReader.token() == U::ARG_VALUE && !strcmp(argReader.str(), "help"))
            mainHelp(argReader);        
        else
            mainCli(argReader);
        
        return 0;
    }
    catch (U::ArgNoError &) {        
        return 0;
    }
    catch (U::ArgError &e) {
        if (!e.silent())
            fprintf(stderr, "Error reading command line: %s\n%s", e.what(), BLOB_Usage_txt);        
    }
    catch (std::runtime_error &e) {
        fprintf(stderr, "Error: %s\n", e.what());
    }
    return -1;    
}

void mainCli(U::ArgReader &argReader) {
    U::FileRefPrinter stderrPrinter(U::OF_FILE, stderr);
    U::FileRefPrinter stdoutPrinter(U::OF_FILE, stdout);
    
    ExprSet es;    
    std::vector<ExprPtr> exprs;
    
    ReadSignalFlags csvReadFlags = READ_SIGNAL_DEFAULT;
    bool outRobustness = false;
    bool outSignal = false;
    OutSignalFormat outSignalFormat = OUT_SIGNAL_CSV;
    const char *outSignalName = "Output";
    bool printExpr = false;
    bool printInput = false;    
    bool printCounters = false;
    bool printMessages = false;
    bool evalExprs = true;
    bool bufferSignalFile = false;
    
    PCSignal signal;    
    // argReader should also be positioned at the first argument.
    while (argReader.token() != U::ARG_EOF) {        
        switch (argReader.token()) {
        case U::ARG_KEY: {
            if (!strcmp(argReader.str(), "-f")) {
                U::TimeCounter c(COUNTER_ID_READ_EXPR_TIME);
                SExprParser<U::SLexer<U::StrReader>> parser(es, U::OF_READER, argReader.readStr());
                exprs.emplace_back(parser.parse());
            }
            else if (!strcmp(argReader.str(), "-ff")) {
                U::TimeCounter c(COUNTER_ID_READ_EXPR_TIME);                    
                U::File file;
                U::FileRef fileRef;
                const char *fileName = argReader.readStr();
                if (!strcmp(fileName, "-")) 
                    fileRef = U::FileRef(stdin);
                else {
                    file = U::File(fileName, "r");
                    fileRef = U::FileRef(file);
                }
                    
                SExprParser<U::SLexer<U::FileRefReader>> parser(es, U::OF_READER, U::OF_FILE, fileRef);
                exprs.emplace_back(parser.parse());
            }                
            else if (!strcmp(argReader.str(), "-isf")) {
                const char *flags = argReader.readStr();
                try {
                    csvReadFlags = readSignalFlagsFromStr(flags);    
                } catch (std::runtime_error &e) {
                    throw U::ArgError(e.what());
                }
            }
            else if (!strcmp(argReader.str(), "-or")) {                    
                outRobustness = argReader.readInt();
            }
            else if (!strcmp(argReader.str(), "-r")) {                    
                outRobustness = true;
            }
            else if (!strcmp(argReader.str(), "-os")) {                    
                outSignal = argReader.readInt();
            }
            else if (!strcmp(argReader.str(), "-osf")) {
                const char *format = argReader.readStr();
                switch (*format) {
                case 'c':
                    outSignalFormat = OUT_SIGNAL_CSV;
                    break;
                    
                case 'g':
                    outSignalFormat = OUT_SIGNAL_GNUPLOT;
                    break;
                    
                case 'd':
                    outSignalFormat = OUT_SIGNAL_DEBUG;
                    break;
                    
                default:
                    std::stringstream ss;
                    ss << "Unknown signal format: '" << *format << "'.";
                    throw U::ArgError(ss.str());
                }
            }
            else if (!strcmp(argReader.str(), "-osn")) {
                outSignalName = argReader.readStr();
            }
            else if (!strcmp(argReader.str(), "-pf")) {
                printExpr = true;
            }
            else if (!strcmp(argReader.str(), "-pi")) {
                printInput = true;
            }
            else if (!strcmp(argReader.str(), "-pc")) {
                printCounters = true;
            }
            else if (!strcmp(argReader.str(), "-pm")) {
                printMessages = true;
            }
            else if (!strcmp(argReader.str(), "-de")) {
                evalExprs = argReader.readInt();
            }
            else if (!strcmp(argReader.str(), "-db")) {
                bufferSignalFile = argReader.readInt();
            }
            else if (!strcmp(argReader.str(), "-v")) {
                fprintf(stdout, "StlEval CLI version %s, library version %s.\n", STLE_CLI_VERSION, stleVersion());
                throw U::ArgNoError();
            }
            else if (!strcmp(argReader.str(), "-h")) {
                fputs(BLOB_Usage_txt, stdout);
                throw U::ArgNoError();
            }
            else {
                std::stringstream ss;
                ss << "Unknown option: '" << argReader.str() << "'.";
                throw U::ArgError(ss.str());
            }
            break;
        }
        
        case U::ARG_VALUE: {
            U::TimeCounter c(COUNTER_ID_READ_SIGNAL_TIME);
            const char *fileName = argReader.str();            
            U::File file;
            U::FileRef fileRef;
            if (!strcmp(fileName, "-")) 
                fileRef = U::FileRef(stdin);
            else {
                file = U::File(fileName, "r");
                fileRef = U::FileRef(file);
            }
                
            if (file.file() && bufferSignalFile) {
                size_t fileSize = static_cast<size_t>(file.size());
                std::unique_ptr<char[]> buf = std::make_unique<char[]>(fileSize + 1);                    
                size_t readBytes = fread(buf.get(), 1, fileSize, file.file());
                if (file.error()) {
                    std::stringstream ss;
                    ss << "Error reading file '" << fileName << "'.";
                    throw U::IOError(ss.str());
                }
                // Note that fread can clobber the buffer past the number of bytes read (if the requested number was larger).
                buf[readBytes] = '\0';
                U::CsvReader<U::StrReader> csvReader(U::OF_READER, buf.get());
                signal.add(readPCSignalCsv(csvReader, csvReadFlags));
            }
            else {
                U::CsvReader<U::FileReader<U::FileRef>> csvReader(U::OF_READER, U::OF_FILE, fileRef);
                signal.add(readPCSignalCsv(csvReader, csvReadFlags));
            }
                            
            break;
        }

        default:
            throw std::runtime_error("Unknown argument type.");                
        }
        
        argReader.read();
    }
    
    if (exprs.empty())
        throw U::ArgError("At least one formula should be given.");
    if (signal.empty())
        throw U::ArgError("A signal should be given.");
    
    if (printInput) {            
        if (printMessages)
            stderrPrinter << "Input signal:\n";
        PCSignalPrinter sp(signal);
        sp.segmentSeparator = "\n";
        stderrPrinter << sp << "\n\n";
    }
    
    SignalVars vars(VARS_XN, signal.size());
    // TODO: How not to initialize the monitor that we won't use?
    PCMonitor monitor(signal, vars, es);
    PCRobMonitor robMonitor(signal, vars, es);
    
    for (size_t i = 0, size = exprs.size(); i < size; ++i) {
        ExprPtr exprI = exprs[i];
        ExprPtr rewrittenExprI;
        {
            U::TimeCounter c(COUNTER_ID_REWRITE_TIME);
            rewrittenExprI = rewriteForPCMonitor(es, exprI);            
        }        
        if (printExpr) {
            if (printMessages)
                stderrPrinter << "Formula:\n";
            stderrPrinter << *exprI << "\n";
            if (printMessages)
                stderrPrinter << "Rewritten formula:\n";
            stderrPrinter << *rewrittenExprI << "\n";        
        }
        
        const PCSeries *outSeries = nullptr;
        Space result(0);
        
        if (evalExprs) {
            {
                U::TimeCounter c(COUNTER_ID_MONITOR_TIME);            
                if (!outRobustness)
                    outSeries = &monitor.makeOutput(rewrittenExprI, false);
                else
                    outSeries = &robMonitor.makeOutput(rewrittenExprI, false);
                result = outSeries->at(0).value();
            }
            if (!outSignal)
                printf("%g\n", result.value());
            else {
                switch (outSignalFormat) {
                case OUT_SIGNAL_CSV: {
                    PCSeriesCsvPrinter sp(*outSeries);                    
                    stdoutPrinter << sp << "\n\n";
                    break;
                }
                case OUT_SIGNAL_GNUPLOT: {
                    PCSeriesGnuplotPrinter gp(*outSeries);
                    gp.name = outSignalName;
                    stdoutPrinter << gp << "\n\n";
                    break;
                }
                case OUT_SIGNAL_DEBUG: {
                    PCSeriesPrinter sp(*outSeries);
                    sp.separator = "\n";
                    stdoutPrinter << sp << "\n\n";
                    break;
                }
                }
            }
        }        
    }            
    
    if (printCounters) {           
        U::Counters::setNameDouble(COUNTER_ID_MONITOR_TIME, "Time spent in monitor, s");
        U::Counters::setNameDouble(COUNTER_ID_REWRITE_TIME, "Time spent in rewriter, s");
        U::Counters::setNameDouble(COUNTER_ID_READ_SIGNAL_TIME, "Time spent in signal parser, s");
        U::Counters::setNameDouble(COUNTER_ID_READ_EXPR_TIME, "Time spent in formula parser, s");
        U::Counters::print(stderrPrinter);            
    }
}

class CliRunCmdVisitor : public Cmd::RunCmdVisitor {
public:
    CliRunCmdVisitor(Cmd::State &state) : Cmd::RunCmdVisitor(state) {
    }
    
    void visitVersion(const Cmd::VersionCmd &) {
        _state->setStringResult(stleVersion());
    }
};

void mainCmd(U::ArgReader &) {
    // TODO: Some command line options.
    // TODO: Need tests for cli and cmd interfaces.
    U::FileRefPrinter stdoutPrinter(U::OF_FILE, stdout);
     
    Cmd::CmdRunner<Cmd::PCState, CliRunCmdVisitor, U::FileRefPrinter> runnner(U::OF_PRINTER, U::OF_FILE, stdout);
    runnner.runReader<U::FileRefReader>(U::OF_READER, U::OF_FILE, stdin);
}

void mainHelp(U::ArgReader &argReader) {
    argReader.read();
    if (argReader.token() == U::ARG_EOF)
        fputs(BLOB_Topics_txt, stdout);    
    else if (argReader.token() == U::ARG_VALUE && !strcmp(argReader.str(), "usage"))
        fputs(BLOB_Usage_txt, stdout);    
    else if (argReader.token() == U::ARG_VALUE && !strcmp(argReader.str(), "formula"))
        fputs(BLOB_Formula_txt, stdout);
    else if (argReader.token() == U::ARG_VALUE && !strcmp(argReader.str(), "cmd"))
        fputs(BLOB_Cmd_txt, stdout);
    else {
        fprintf(stderr, "Unknown help topic '%s'.\n%s", argReader.str(), BLOB_Topics_txt);
        throw U::ArgError("", true);
    }
}
