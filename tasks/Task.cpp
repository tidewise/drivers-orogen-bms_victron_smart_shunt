/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"
#include <bms_victron_smart_shunt/Driver.hpp>
#include <iodrivers_base/ConfigureGuard.hpp>

using namespace bms_victron_smart_shunt;
using namespace iodrivers_base;

bms_victron_smart_shunt::Task::Task(std::string const& name)
    : TaskBase(name)
{
}

bms_victron_smart_shunt::Task::~Task()
{
}

bool bms_victron_smart_shunt::Task::configureHook()
{
    iodrivers_base::ConfigureGuard guard(this);
    Driver* driver = new Driver();
    if (!_io_port.get().empty())
        driver->openURI(_io_port.get());
    setDriver(driver);
    if (!TaskBase::configureHook())
        return false;
    guard.commit();
    return true;
}
bool bms_victron_smart_shunt::Task::startHook()
{
    if (!TaskBase::startHook())
        return false;
    return true;
}
void bms_victron_smart_shunt::Task::updateHook()
{
    TaskBase::updateHook();
}
void bms_victron_smart_shunt::Task::errorHook()
{
    TaskBase::errorHook();
}
void bms_victron_smart_shunt::Task::stopHook()
{
    TaskBase::stopHook();
}
void bms_victron_smart_shunt::Task::cleanupHook()
{
    TaskBase::cleanupHook();
}

void bms_victron_smart_shunt::Task::processIO()
{
    Driver* driver = static_cast<bms_victron_smart_shunt::Driver*>(mDriver);
    SmartShuntFeedback feedback = driver->processOne();
    if (driver->packetsCounter() >= 2) {
        _smart_shunt_feedback.write(feedback);
    }
}
