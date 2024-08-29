/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"
#include <bms_victron_smart_shunt/Driver.hpp>
#include <iodrivers_base/ConfigureGuard.hpp>
#include <power_base/ACSourceStatus.hpp>

using namespace bms_victron_smart_shunt;
using namespace iodrivers_base;
using namespace power_base;

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
    m_packets_to_compose_a_full_feedback = _packets_to_compose_a_full_feedback.get();
    m_max_current = 0;
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

static BatteryStatus toBatteryStatus(SmartShuntFeedback const& feedback,
    double max_current)
{
    BatteryStatus battery_status;
    battery_status.time = feedback.timestamp;
    battery_status.temperature = feedback.temperature;
    battery_status.charge = feedback.state_of_charge / 100;
    battery_status.current = feedback.current;
    battery_status.voltage = feedback.voltage;
    battery_status.max_current = max_current;
    return battery_status;
}

void bms_victron_smart_shunt::Task::updateMaxCurrent(double actual_current)
{
    if (actual_current > m_max_current) {
        m_max_current = actual_current;
    }
}

void bms_victron_smart_shunt::Task::processIO()
{
    Driver* driver = static_cast<bms_victron_smart_shunt::Driver*>(mDriver);
    SmartShuntFeedback feedback =
        driver->processOne(m_packets_to_compose_a_full_feedback);
    updateMaxCurrent(feedback.current);
    if (driver->packetsCounter() >= m_packets_to_compose_a_full_feedback) {
        _smart_shunt_feedback.write(feedback);
        _battery_status.write(toBatteryStatus(feedback, m_max_current));
    }
}
