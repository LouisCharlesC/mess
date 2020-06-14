/**
 * @file flow.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2020-05-09
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <mess/mess.h>

struct Source:
	mess::IsALocalVariable
{};
struct DataSize:
	mess::IsALocalVariable
{};
struct OriginalData:
	mess::Invokes<&ex::Source::get>,
	mess::OnInstance<Source>,
	mess::WithArgument<DataSize>
{};
struct ScannedDataContainer:
	mess::IsALocalVariable
{};
struct DoScan:
	mess::IsTheResultOfALocalInvocable,
	mess::WithArguments<OriginalData, ScannedDataContainer>
{};
// struct ScannedData:
// 	// mess::Is<ScannedDataContainer>,
// 	// mess::After<DoScan>
// {};
struct Check:
	mess::IsTheResultOfALocalInvocable,
	mess::WithArguments<ScannedData, OriginalData>
{};